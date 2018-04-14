// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/token.h"
#include "lexer/strings.h"
#include "DEFS.h"

static char *reserved = " new return break continue switch case if elif else "
  "default while do for enum try catch finally val var native ";

Txpos *token_blanks(Txpos *tx) {
  char *p = txpos_start(tx);
  char *end = txpos_end(tx);
  size_t nline = txpos_nline(tx);
  size_t nchar = txpos_nchar(tx);

  char *rune;
  int len;
  for (;;) {
    while (p < end && *p <= ' ') {
      if (!*p)
        THROW "Unexpected end before txpos_end()" _THROW
      if (*p == '\n') {
        ++nline;
        nchar = 0;
      } else {
        ++nchar;
      }
      ++p;
    }

    if (p < end - 1 && *p == '/') {
      if (*(p + 1) == '/') {
        while (p < end && *p != '\n') {
          ++p;
        }
        if (p != txpos_end(tx)) {
          ++p;
          ++nline;
          nchar = 0;
        }
        continue;
      } else if (*(p + 1) == '*') {
        while (p < end && (*p != '*' || *(p + 1) != '/')) {
          if (*p == '\n') {
            ++p;
            ++nline;
            nchar = 0;
          } else {
            rune = str_next_rune(p);
            len = strlen(rune);
            if (!len)
              TH(tx) "Expected '*/'" _TH
            p += len;
            ++nchar;
          }
        }
        if (p >= end)
          TH(tx) "Expected '*/'" _TH

        p += 2;
        nchar += 2;
        continue;
      }
    }

    break;
  }

  return txpos_move(tx, p, nline, nchar);
}

Txpos *token_skip_stat(Txpos *tx) {
  int brackets = 0;
  char *p;
  for (;;) {
    p = txpos_start(tx);

    if (p == txpos_end(tx)) {
      break;
    }

    char ch = *p;
    if (ch == ';' && brackets == 0) {
      tx = txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
      break;
    }

    if (ch <= ' ') {
      tx = token_blanks(tx);
      continue;
    }

    if (ch == '/') {
      Txpos *r = token_blanks(tx);
      if (txpos_neq(tx, r)) {
        tx = r;
        continue;
      }
    }

    if (ch == '\n') {
      tx = txpos_move(tx, p + 1, txpos_nline(tx) + 1, 0);
      continue;
    }

    if (ch == '"' || ch == '`' || ch == '\'') {
      tx = txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
      tx = strings_skip(tx, ch);
      continue;
    }

    if (ch == '{') {
      ++brackets;
    } else if (ch == '}' && brackets) {
      --brackets;
    }

    tx = txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
  }

  return token_blanks(tx);
}

Txpos *token_cconst0(Txpos *tx, char value) {
  char *p = txpos_start(tx);
  if (*p == value) {
    return txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
  }
  return tx;
}

Txpos *token_cconst(Txpos *tx, char value) {
  char *p = txpos_start(tx);
  if (*p == value) {
    return token_blanks(
      txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1)
    );
  }
  return tx;
}

Txpos *token_const(Txpos *tx, char *value) {
  char *p = txpos_start(tx);
  if (str_starts(p, value)) {
    size_t size = strlen(value);
    return token_blanks(
      txpos_move(tx, p + size, txpos_nline(tx), txpos_nchar(tx) + size)
    );
  }
  return tx;
}

Txpos *token_csplit0(char *r, Txpos *tx, char *values) {
  char *p = txpos_start(tx);
  char value;
  while ((value = *values++)) {
    if (*p == value) {
      *r = value;
      return txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
    }
  }
  return tx;
}

Txpos *token_csplit(char *r, Txpos *tx, char *values) {
  char *p = txpos_start(tx);
  char value;
  while ((value = *values++)) {
    if (*p == value) {
      *r = value;
      return token_blanks(
        txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1)
      );
    }
  }
  return tx;
}

Txpos *token_split(char **r, Txpos *tx, char *values) {
  char *p = txpos_start(tx);
  EACH(str_csplit(values, ' '), char, value) {
    if (str_starts(p, value)) {
      *r = value;
      size_t size = strlen(value);
      return token_blanks(
        txpos_move(tx, p + size, txpos_nline(tx), txpos_nchar(tx) + size)
      );
    }
  }_EACH
  return tx;
}

Txpos *token_list(
  Arr **list, Txpos *tx, char close, Txpos *(*read)(void **, Txpos *)
) {
  Arr *l = arr_new();

  Txpos *r = token_cconst(tx, close);
  if (txpos_eq(tx, r)) {
    void *e;
    r = read(&e, tx);
    if (txpos_eq(tx, r))
      TH(tx) "Expected a valid element" _TH
    tx = r;

    arr_add(l, e);

    for (;;) {
      r = token_cconst(tx, close);
      if (txpos_neq(tx, r)) {
        break;
      } else {
        r = token_cconst(tx, ',');
        if (txpos_eq(tx, r))
          TH(tx) "Expected ',' (comma)" _TH
        tx = r;

        r = read(&e, tx);
        if (txpos_eq(tx, r))
          TH(tx) "Expected a valid element" _TH
        tx = r;

        arr_add(l, e);
      }
    }
  }

  *list = l;
  return r;
}

Txpos *token_fn_list(Arr **list, Txpos *tx, Txpos *(*read)(void **, Txpos *)) {
  Txpos *start = tx;
  Arr *l = arr_new();

  Txpos *r = token_const(tx, "->");
  if (txpos_eq(tx, r)) {
    void *e;
    r = read(&e, tx);
    if (txpos_eq(tx, r)) {
      return start;
    }
    tx = r;

    arr_add(l, e);

    for (;;) {
      r = token_const(tx, "->");
      if (txpos_neq(tx, r)) {
        break;
      } else {
        r = token_cconst(tx, ',');
        if (txpos_eq(tx, r)) {
          return start;
        }
        tx = r;

        r = read(&e, tx);
        if (txpos_eq(tx, r)) {
          return start;
        }
        tx = r;

        arr_add(l, e);
      }
    }
  }

  *list = l;
  return r;
}

Txpos *token_id(char **id, Txpos *tx) {
  char *start = txpos_start(tx);
  char *p = start;

  char ch = *p++;
  if (
    !txpos_at_end(tx) &&
    (
      (ch >= 'A' && ch <= 'Z') ||
      (ch >= 'a' && ch <= 'z') ||
      ch == '$'
    )
  ) {
    ch = *p++;
    while (
      !txpos_at_end(tx) &&
      (
        (ch >= 'a' && ch <= 'z') ||
        (ch >= '0' && ch <= '9') ||
        (ch >= 'A' && ch <= 'Z') ||
        ch == '$'
      )
    ) {
      ch = *p++;
    }
    --p;
    int len = p - start;
    char *i = ATOMIC(len + 1);
    memcpy(i, start, len);
    i[len] = 0;
    *id = i;

    return token_blanks(
      txpos_move(tx, p, txpos_nline(tx), txpos_nchar(tx) + len)
    );
  } else {
    *id = "";
    return tx;
  }
}

Txpos *token_directive(Txpos *tx, char *value) {
  char *start = txpos_start(tx);
  char *p = start;

  char ch = *p++;
  if (!txpos_at_end(tx) && ch == '_' ) {

    ch = *p++;
    while (
      !txpos_at_end(tx) &&
      ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    ) {
      ch = *p++;
    }
    --p;

    int len = p - start;
    char *d = ATOMIC(len + 1);
    memcpy(d, start, len);
    d[len] = 0;

    if (!strcmp(d, value)) {
      return token_blanks(
        txpos_move(tx, p, txpos_nline(tx), txpos_nchar(tx) + len)
      );
    }
  }

  return tx;
}

Txpos *token_path(char **path, Txpos *tx) {
  char *start = txpos_start(tx);
  char *p = start;

  char ch = *p++;
  if (
    txpos_at_end(tx) ||
    ((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z'))
  ) {
    return tx;
  }

  for (;;) {
    ch = *p++;
    if (
      !txpos_at_end(tx) &&
      ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '/')
    ) {
      continue;
    }
    break;
  }
  --p;
  int len = p - start;
  char *ptmp = ATOMIC(len + 1);
  memcpy(ptmp, start, len);
  ptmp[len] = 0;
  *path = ptmp;

  return token_blanks(
    txpos_move(tx, p, txpos_nline(tx), txpos_nchar(tx) + len)
  );
}

Txpos *token_point_id(char **id, Txpos *tx) {
  Txpos *r;
  Buf *bf = buf_new();
  char *i;
  Txpos *prepoint = NULL;
  for (;;) {
    if (txpos_eq(tx, r = token_id(&i, tx))) {
      if (prepoint) {
        tx = prepoint;
      }
      break;
    }
    tx = r;
    if (prepoint) {
      buf_cadd(bf, '.');
    }
    buf_add(bf, i);
    if (txpos_eq(tx, r = token_cconst(tx, '.'))) {
      break;
    }
    prepoint = tx;
    tx = r;
  }

  *id = buf_to_str(bf);
  return tx;
}

Txpos *token_bool(char **value, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = token_const(tx, "true"))) {
    *value = "true";
    return r;
  }
  if (txpos_neq(tx, r = token_const(tx, "false"))) {
    *value = "false";
    return r;
  }
  return tx;
}

Txpos *token_lunary(char **op, Txpos *tx) {
  Txpos *r;
  char ch;

  if (txpos_neq(tx, r = token_csplit(&ch, tx, "!"))) {
    *op = " ";
    **op = ch;
    return r;
  }

  if (txpos_neq(tx, r = token_split(op, tx, "++ --"))) {
    return r;
  }

  return tx;
}

Txpos *token_runary(char **op, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = token_split(op, tx, "++ --"))) {
    return r;
  }

  return tx;
}

Txpos *token_binary(char **op, Txpos *tx) {
  char *ch1 = "+-*/%^><&|";
  char *chs = "== != <= <= && || ^^ >>> << >> ?:";

  Txpos *r;
  char ch;

  if (txpos_neq(tx, r = token_csplit(&ch, tx, ch1))) {
    *op = " ";
    **op = ch;
    return r;
  }

  if (txpos_neq(tx, r = token_split(op, tx, chs))) {
    return r;
  }

  return tx;
}

bool token_is_reserved(char *id) {
  return str_index(reserved, str_printf(" %s ", id)) != -1;
}

Txpos *token_valid_id(char **id, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = token_id(id, tx))) {
    if (token_is_reserved(*id))
      TH(tx) "'%s' is a reserved word", *id _TH
    tx = r;
  }
  return tx;
}

Txpos *token_native(char **text, Txpos *tx, char *mark) {
  int len_mark = strlen(mark);
  char *p = txpos_start(tx);
  char *start = p;
  char *end = txpos_end(tx);
  int nchar = txpos_nchar(tx);
  int nline = txpos_nline(tx);

  for (;;) {
    if (!*p)
      THROW "Unexpected end before txpos_end()" _THROW
    if (p == end)
      TH(tx) "Mark end of native '%s' is missing", mark _TH

    if (!memcmp(p, mark, len_mark)) {
      break;
    }
    if (*p == '\n') {
      ++nline;
      nchar = 0;
    } else {
      ++nchar;
    }
    ++p;
  }

  *text = str_sub(start, 0, p - start);
  return token_blanks(
    txpos_move(tx, p + len_mark, nline, nchar + len_mark)
  );
}
