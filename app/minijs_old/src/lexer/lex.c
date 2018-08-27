// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include <string.h>
#include "dmc/ct/Lchar.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/List.h"
#include "dmc/Arr.h"
#include "dmc/DEFS.h"
#include "ast/ops.h"
#include "lexer/Tx.h"
#include "lexer/lex.h"
#include "DEFS.h"

static char *reserved = " new return break continue case with if elif else "
  "while for enum try catch finally val var nat native true false null this "
  "super ";

bool lex_end(Tx *tx) {
  return !*tx_start(tx);
}

Tx *lex_blanks(Tx *tx) {
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
            len = str_len(rune);
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

Tx *lex_no_blanks(char **string, Tx *tx) {
  char *start = tx_start(tx);
  char *p = start;

  while (*p > ' ') {
    ++p;
  }

  int len = p - start;
  char *s = ATOMIC(len + 1);
  memcpy(s, start, len);
  s[len] = 0;
  *string = s;

  return tx_move(tx, p, tx_nline(tx), tx_nchar(tx) + len);
}

Tx *lex_char(char *ch, Tx *tx) {
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

Tx *lex_native(char **text, Tx *tx, char *mark) {
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
  return tx_move(tx, p + len_mark, nline, nchar + len_mark);
}

Tx *lex_cconst(Tx *tx, char ch) {
  if (*tx_start(tx) == ch) {
    return tx_move(tx, tx_start(tx) + 1, tx_nline(tx), tx_nchar(tx) + 1);
  }
  return tx;
}

Tx *lex_const(Tx *tx, char *s) {
  char *p = tx_start(tx);
  if (str_starts(p, s)) {
    size_t size = str_len(s);
    tx = tx_move(tx, p + size, tx_nline(tx), tx_nchar(tx) + size);
  }
  return tx;
}

Tx *lex_id(char **rt, Tx *tx) {
  char *start = tx_start(tx);
  char *p = start;

  char ch = *p;
  if (
    (ch >= 'A' && ch <= 'Z') ||
    (ch >= 'a' && ch <= 'z') ||
    ch == '$'
  ) {
    ++p;
    ch = *p;
    while (
      (ch >= 'a' && ch <= 'z') ||
      (ch >= '0' && ch <= '9') ||
      (ch >= 'A' && ch <= 'Z') ||
      ch == '$'
    ) {
      ++p;
      ch = *p;
    }
    int len = p - start;
    char *i = ATOMIC(len + 1);
    memcpy(i, start, len);
    i[len] = 0;
    *rt = i;

    return tx_move(tx, p, tx_nline(tx), tx_nchar(tx) + len);
  }
  return tx;
}

Tx *lex_valid_id(char **id, Tx *tx) {
  Tx *r = lex_id(id, tx);
  lex_test_reserved(tx, *id);
  return r;
}

Tx *lex_csplit(char *r, Tx *tx, char *values) {
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

Tx *lex_split(char **r, Tx *tx, char *values) {
  char *p = tx_start(tx);
  EACH(str_csplit(values, ' '), char, value) {
    if (str_starts(p, value)) {
      *r = value;
      size_t size = strlen(value);
      return tx_move(tx, p + size, tx_nline(tx), tx_nchar(tx) + size);
    }
  }_EACH
  return tx;
}

static Tx *list_arr(
  Tx *tx, char end, Tx *(*relement)(void **, Tx *tx), void (*append)(void *)
) {
  Tx *r;

  if (tx_neq(tx, r = lex_cconst(tx, end))) {
    return r;
  }

  void *element;
  for (;;) {
    if (tx_eq(tx, r = relement(&element, tx)))
      TH(tx) "Expected a list element" _TH
    tx = lex_blanks(r);
    append(element);

    if (tx_neq(tx, r = lex_cconst(tx, end))) {
      return r;
    }

    if (tx_eq(tx, r = lex_cconst(tx, ',')))
      TH(tx) "Expected '%c'", ',' _TH
    tx = lex_blanks(r);
  }
}

Tx *lex_list(
  List **rt, Tx *tx, char end, Tx *(*relement)(void **, Tx *tx)
) {
  List *l = list_new();

  /**/void append(void *e) { l = list_cons(l, e); }
  tx = list_arr(tx, end, relement, append);

  *rt = list_reverse(l);
  return tx;
}

Tx *lex_arr(
  Arr **rt, Tx *tx, char end, Tx *(*relement)(void **, Tx *tx)
) {
  Arr *a = arr_new();

  /**/void append(void *e) { arr_add(a, e); }
  tx = list_arr(tx, end, relement, append);

  *rt = a;
  return tx;
}

Tx *lex_fn_list(Achar **list, Tx *tx, Tx *(*read)(char **, Tx *)) {
  Tx *start = tx;
  Achar *l = achar_new();

  Tx *r = lex_blanks(lex_const(tx, "->"));
  if (tx_eq(tx, r)) {
    char *e;
    r = read(&e, tx);
    if (tx_eq(tx, r)) {
      return start;
    }
    tx = lex_blanks(r);

    achar_add(l, e);

    for (;;) {
      r = lex_const(tx, "->");
      if (tx_neq(tx, r)) {
        break;
      } else {
        r = lex_cconst(tx, ',');
        if (tx_eq(tx, r)) {
          return start;
        }
        tx = lex_blanks(r);

        r = read(&e, tx);
        if (tx_eq(tx, r)) {
          return start;
        }
        tx = lex_blanks(r);

        achar_add(l, e);
      }
    }
  }

  *list = l;
  return r;
}

void lex_test_reserved(Tx *tx, char *id) {
  if (str_index(reserved, str_cat(" ", id, " ", NULL)) != -1)
    TH(tx) "'%s' is a reserved word", id _TH
}

void lex_test_symbol(Tx *tx, Achar *symbols, char *id) {
  if (ichar_contains(achar_to_it(symbols), id))
    TH(tx) "Identifier '%s' is duplicate", id _TH
}

Tx *lex_generic_id(char **id, Achar **generics, Tx *tx) {
  Tx *r;
  if (tx_eq(tx, r = lex_id(id, tx))) {
    return tx;
  }
  tx = lex_blanks(r);

  if (tx_eq(tx, r = lex_cconst(tx, '<'))) {
    *generics = achar_new();
    return tx;
  }
  tx = lex_blanks(r);

  Achar *gs = achar_new();
  tx = lex_arr((Arr **)gs, tx, '>', (Tx*(*)(void**, Tx*))lex_valid_id);
  *generics = gs;

  if (!achar_size(*generics))
    TH(r) "Empty generics brackets '<>' is not allowed" _TH

  return tx;
}

Tx *lex_bool(char **value, Tx *tx) {
  Tx *r;
  if (tx_neq(tx, r = lex_id(value, tx))) {
    if (strcmp(*value, "true") && strcmp(*value, "false")) {
      return tx;
    }
    return r;
  }
  return tx;
}

Tx *lex_lunary(char **op, Tx *tx) {
  Tx *r;
  char ch;

  if (tx_neq(tx, r = lex_split(op, tx, ops_u2()))) {
    return r;
  }

  if (tx_neq(tx, r = lex_csplit(&ch, tx, ops_u1()))) {
    char *o = ATOMIC(2);
    *o = ch;
    *(o + 1) = 0;
    *op = o;
    return r;
  }

  return tx;
}

Tx *lex_runary(char **op, Tx *tx) {
  Tx *r;
  if (tx_neq(tx, r = lex_split(op, tx, ops_u2()))) {
    return r;
  }

  return tx;
}

static Tx *bin(char **op, Tx *tx, char *ops1, char *ops2) {
  Tx *r;
  char ch;

  if (tx_neq(tx, r = lex_split(op, tx, ops2))) {
    return r;
  }

  if (tx_neq(tx, r = lex_csplit(&ch, tx, ops1))) {
    char *o = ATOMIC(2);
    *o = ch;
    *(o + 1) = 0;
    *op = o;
    return r;
  }

  return tx;
}

inline
Tx *lex_binary(char **op, Tx *tx) {
  return bin(op, tx, ops_b1(), ops_b2());
}

Tx *lex_assign(char **op, Tx *tx) {
  return bin(op, tx, ops_a1(), ops_a2());
}
