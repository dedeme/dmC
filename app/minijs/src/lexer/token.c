// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/token.h"
#include "lexer/strings.h"
#include "DEFS.h"

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
  char *p = txpos_start(tx);
  char *end = txpos_end(tx);

  for (;;) {
    if (p == end) {
      break;
    }

    char ch = *p;
    if (ch == ';') {
      tx = txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
      break;
    }

    if (ch <= ' ') {
      tx = token_blanks(tx);
      continue;
    }

    if (ch == '/') {
      Txpos *r = token_blanks(tx);
      if (!txpos_eq(tx, r)) {
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

    tx = txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 1);
  }

  return token_blanks(tx);
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

Txpos *token_list(
  Arr/*char*/ **list, Txpos *tx, char close, Txpos *(*read)(void **, Txpos *)
) {
  Arr/*char*/ *l = arr_new();

  Txpos *r = token_cconst(tx, close);
  if (txpos_eq(tx, r)) {
    void *e;
    r = read(&e, tx);
    if (txpos_eq(tx, r))
      TH(tx) "Expected a valid identifier" _TH
    tx = r;

    arr_add(l, e);

    for (;;) {
      r = token_cconst(tx, close);
      if (txpos_neq(tx, r)) {
        break;
      } else {
        r = token_cconst(tx, ',');
        if (txpos_eq(tx, r))
          TH(tx) "Expected ','" _TH
        tx = r;

        r = read(&e, tx);
        if (txpos_eq(tx, r))
          TH(tx) "Expected a valid identifier" _TH
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

  char ch = *p;
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
