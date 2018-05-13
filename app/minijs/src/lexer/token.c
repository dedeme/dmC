// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/token.h"
#include "DEFS.h"
#include "ast/ops.h"

static char *reserved = " new return break continue with if elif else "
  "while for enum try catch finally val var native true false null ";

Tx *token_blanks(Tx *tx) {
  char *p = tx_start(tx);
  size_t nline = tx_nline(tx);
  size_t nchar = tx_nchar(tx);

  char *rune;
  int len;
  for (;;) {
    while (*p && *p <= ' ') {
      if (*p == '\n') {
        ++nline;
        nchar = 0;
      } else {
        ++nchar;
      }
      ++p;
    }

    if (*(p + 1) && *p == '/') {
      if (*(p + 1) == '/') {
        while (*p && *p != '\n') {
          ++p;
        }
        if (*p) {
          ++p;
          ++nline;
          nchar = 0;
        }
        continue;
      } else if (*(p + 1) == '*') {
        while (*p && (*p != '*' || *(p + 1) != '/')) {
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
        if (!*p || !*(p + 1))
          TH(tx) "Expected '*/'" _TH

        p += 2;
        nchar += 2;
        continue;
      }
    }

    break;
  }

  return tx_move(tx, p, nline, nchar);
}

/*
Tx *token_skip_block(Tx *tx) {
  int brackets = 0;
  char *p;
  for (;;) {
    p = tx_start(tx);

    if (!*p) {
      break;
    }

    char ch = *p;
    if (ch == '}' && brackets == 0) {
      tx = tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1);
      break;
    }

    if (ch <= ' ') {
      tx = token_blanks(tx);
      continue;
    }

    if (ch == '/') {
      Tx *r = token_blanks(tx);
      if (tx_neq(tx, r)) {
        tx = r;
        continue;
      }
    }

    if (ch == '\n') {
      tx = tx_move(tx, p + 1, tx_nline(tx) + 1, 0);
      continue;
    }

    if (ch == '"' || ch == '`' || ch == '\'') {
      tx = tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1);
      tx = strings_skip(tx, ch);
      continue;
    }

    if (ch == '{') {
      ++brackets;
    } else if (ch == '}' && brackets) {
      --brackets;
    }

    tx = tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1);
  }

  return token_blanks(tx);
}
*/

Tx *token_char(char *ch, Tx *tx) {
  char *p = tx_start(tx);
  size_t nline = tx_nline(tx);
  size_t nchar = tx_nchar(tx);

  if (!*p) {
    return tx;
  }
  *ch = *p;
  if (*p == '\n') {
    return tx_move(tx, ++p, ++nline, 0);
  }
  return tx_move(tx, ++p, nline, ++nchar);
}

Tx *token_cconst0(Tx *tx, char value) {
  char *p = tx_start(tx);
  if (*p == value) {
    return tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1);
  }
  return tx;
}

Tx *token_cconst(Tx *tx, char value) {
  char *p = tx_start(tx);
  if (*p == value) {
    return token_blanks(
      tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1)
    );
  }
  return tx;
}

Tx *token_const(Tx *tx, char *value) {
  char *p = tx_start(tx);
  if (str_starts(p, value)) {
    size_t size = strlen(value);
    return token_blanks(
      tx_move(tx, p + size, tx_nline(tx), tx_nchar(tx) + size)
    );
  }
  return tx;
}

Tx *token_csplit0(char *r, Tx *tx, char *values) {
  char *p = tx_start(tx);
  char value;
  while ((value = *values++)) {
    if (*p == value) {
      *r = value;
      return tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1);
    }
  }
  return tx;
}

Tx *token_csplit(char *r, Tx *tx, char *values) {
  char *p = tx_start(tx);
  char value;
  while ((value = *values++)) {
    if (*p == value) {
      *r = value;
      return token_blanks(
        tx_move(tx, p + 1, tx_nline(tx), tx_nchar(tx) + 1)
      );
    }
  }
  return tx;
}

Tx *token_split(char **r, Tx *tx, char *values) {
  char *p = tx_start(tx);
  EACH(str_csplit(values, ' '), char, value) {
    if (str_starts(p, value)) {
      *r = value;
      size_t size = strlen(value);
      return token_blanks(
        tx_move(tx, p + size, tx_nline(tx), tx_nchar(tx) + size)
      );
    }
  }_EACH
  return tx;
}

Tx *token_list(Arr **list, Tx *tx, char close, Tx *(*read)(void **, Tx *)) {
  Arr *l = arr_new();

  Tx *r = token_cconst(tx, close);
  if (tx_eq(tx, r)) {
    void *e;
    r = read(&e, tx);
    if (tx_eq(tx, r))
      TH(tx) "Expected a valid element" _TH
    tx = r;

    arr_add(l, e);

    for (;;) {
      r = token_cconst(tx, close);
      if (tx_neq(tx, r)) {
        break;
      } else {
        r = token_cconst(tx, ',');
        if (tx_eq(tx, r))
          TH(tx) "Expected ',' (comma) or '%c'", close _TH
        tx = r;

        r = read(&e, tx);
        if (tx_eq(tx, r))
          TH(tx) "Expected a valid element" _TH
        tx = r;

        arr_add(l, e);
      }
    }
  }

  *list = l;
  return r;
}

Tx *token_fn_list(Achar **list, Tx *tx, Tx *(*read)(char **, Tx *)) {
  Tx *start = tx;
  Arr *l = arr_new();

  Tx *r = token_const(tx, "->");
  if (tx_eq(tx, r)) {
    char *e;
    r = read(&e, tx);
    if (tx_eq(tx, r)) {
      return start;
    }
    tx = r;

    arr_add(l, e);

    for (;;) {
      r = token_const(tx, "->");
      if (tx_neq(tx, r)) {
        break;
      } else {
        r = token_cconst(tx, ',');
        if (tx_eq(tx, r)) {
          return start;
        }
        tx = r;

        r = read(&e, tx);
        if (tx_eq(tx, r)) {
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

Tx *token_id(char **id, Tx *tx) {
  char *start = tx_start(tx);
  char *p = start;

  char ch = *p++;
  if (
    !tx_at_end(tx) &&
    (
      (ch >= 'A' && ch <= 'Z') ||
      (ch >= 'a' && ch <= 'z') ||
      ch == '$'
    )
  ) {
    ch = *p++;
    while (
      !tx_at_end(tx) &&
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
      tx_move(tx, p, tx_nline(tx), tx_nchar(tx) + len)
    );
  } else {
    *id = "";
    return tx;
  }
}

Tx *token_directive(Tx *tx, char *value) {
  char *start = tx_start(tx);
  char *p = start;

  char ch = *p++;
  if (!tx_at_end(tx) && ch == '_' ) {

    ch = *p++;
    while (
      !tx_at_end(tx) &&
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
        tx_move(tx, p, tx_nline(tx), tx_nchar(tx) + len)
      );
    }
  }

  return tx;
}

Tx *token_path(char **path, Tx *tx) {
  char *start = tx_start(tx);
  char *p = start;

  char ch = *p++;
  if (
    tx_at_end(tx) ||
    ((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z'))
  ) {
    return tx;
  }

  for (;;) {
    ch = *p++;
    if (
      !tx_at_end(tx) &&
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
    tx_move(tx, p, tx_nline(tx), tx_nchar(tx) + len)
  );
}

Tx *token_bool(char **value, Tx *tx) {
  Tx *r;
  if (tx_neq(tx, r = token_id(value, tx))) {
    if (strcmp(*value, "true") && strcmp(*value, "false")) {
      return tx;
    }
    return r;
  }
  return tx;
}

Tx *token_lunary(char **op, Tx *tx) {
  Tx *r;
  char ch;

  if (tx_neq(tx, r = token_split(op, tx, ops_u2()))) {
    return r;
  }

  if (tx_neq(tx, r = token_csplit(&ch, tx, ops_u1()))) {
    char *o = ATOMIC(2);
    *o = ch;
    *(o + 1) = 0;
    *op = o;
    return r;
  }

  return tx;
}

Tx *token_runary(char **op, Tx *tx) {
  Tx *r;
  if (tx_neq(tx, r = token_split(op, tx, ops_u2()))) {
    return r;
  }

  return tx;
}

static Tx *bin(char **op, Tx *tx, char *ops1, char *ops2) {
  Tx *r;
  char ch;

  if (tx_neq(tx, r = token_split(op, tx, ops2))) {
    return r;
  }

  if (tx_neq(tx, r = token_csplit(&ch, tx, ops1))) {
    char *o = ATOMIC(2);
    *o = ch;
    *(o + 1) = 0;
    *op = o;
    return r;
  }

  return tx;
}

inline
Tx *token_binary(char **op, Tx *tx) {
  return bin(op, tx, ops_b1(), ops_b2());
}

inline
Tx *token_assign(char **op, Tx *tx) {
  return bin(op, tx, ops_a1(), ops_a2());
}

bool token_is_reserved(char *id) {
  return str_index(reserved, str_printf(" %s ", id)) != -1;
}

Tx *token_valid_id(char **id, Tx *tx) {
  Tx *r;
  if (tx_neq(tx, r = token_id(id, tx))) {
    if (token_is_reserved(*id))
      TH(tx) "'%s' is a reserved word", *id _TH
    tx = r;
  }
  return tx;
}

Tx *token_generic_id(char **id, Achar **generics, Tx *tx) {
  Tx *r;
  if (tx_eq(tx, r = token_id(id, tx))) {
    return tx;
  }
  tx = r;

  if (tx_eq(tx, r = token_cconst(tx, '<'))) {
    *generics = arr_new();
    return tx;
  }
  tx = r;
  tx = token_list(generics, tx, '>', (Tx*(*)(void**, Tx*))token_valid_id);

  if (!arr_size(*generics))
    TH(r) "Empty generics brackets '<>' is not allowed" _TH

  return tx;
}

Tx *token_native(char **text, Tx *tx, char *mark) {
  int len_mark = strlen(mark);
  char *p = tx_start(tx);
  char *start = p;
  int nchar = tx_nchar(tx);
  int nline = tx_nline(tx);

  for (;;) {
    if (!*p)
      TH(tx) "Mark '%s' is missing", mark _TH

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
    tx_move(tx, p + len_mark, nline, nchar + len_mark)
  );
}
