// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/numbers.h"
#include "lexer/token.h"
#include "DEFS.h"

static Txpos *ddigits00(char **value, Txpos *tx) {
  Buf *bf = buf_new();
  char *p = txpos_start(tx);
  char *start = p;
  char ch = *p++;
  if (ch < '0' || ch > '9') {
    return tx;
  }
  buf_cadd(bf, ch);
  ch = *p++;
  while (ch >= '0' && ch <= '9') {
    buf_cadd(bf, ch);
    ch = *p++;
  }
  size_t len = buf_length(bf);
  *value = buf_to_str(bf);

  return txpos_move(tx, start + len, txpos_nline(tx), txpos_nchar(tx) + len);
}

inline
static Txpos *ddigits0(char **value, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = ddigits00(value, tx))) {
    return token_blanks(r);
  }
  return tx;
}

static Txpos *ddigits(bool *byte, char **value, Txpos *tx) {
  Buf *bf = buf_new();
  *byte = false;
  char *p = txpos_start(tx);
  char *start = p;
  char ch = *p++;
  if (ch < '0' || ch > '9') {
    return tx;
  }
  buf_cadd(bf, ch);
  ch = *p++;
  while (ch >= '0' && ch <= '9') {
    buf_cadd(bf, ch);
    ch = *p++;
  }
  size_t len = buf_length(bf);
  *value = buf_to_str(bf);
  if (ch == 't') {
    *byte = true;
    ++len;
  }
  return token_blanks(
    txpos_move(tx, start + len, txpos_nline(tx), txpos_nchar(tx) + len)
  );
}

static Txpos *hdigits(bool *byte, char **value, Txpos *tx) {
  Buf *bf = buf_new();
  *byte = false;
  char *p = txpos_start(tx);
  char *start = p;

  if (*p != '0'  || *(p + 1) != 'x') {
    return tx;
  }
  p += 2;
  buf_add(bf, "0x");

  char ch = *p++;
  if (
    (ch < '0' || ch > '9') &&
    (ch < 'a' || ch > 'f') &&
    (ch < 'A' || ch > 'F')
  ) {
    return tx;
  }
  buf_cadd(bf, ch);
  ch = *p++;
  while (
    (ch >= '0' && ch <= '9') ||
    (ch >= 'a' && ch <= 'f') ||
    (ch >= 'A' && ch <= 'F')
  ) {
    buf_cadd(bf, ch);
    ch = *p++;
  }
  size_t len = buf_length(bf);
  *value = buf_to_str(bf);
  if (ch == 't') {
    *byte = true;
    ++len;
  }
  return token_blanks(
    txpos_move(tx, start + len, txpos_nline(tx), txpos_nchar(tx) + len)
  );
}

static Txpos *digits(bool *byte, char **value, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = hdigits(byte, value, tx))) {
    return r;
  }
  if (txpos_neq(tx, r = ddigits(byte, value, tx))) {
    return r;
  }
  return tx;
}

Txpos *numbers_byte(char **value, Txpos *tx) {
  Txpos *r;
  bool byte;
  if (txpos_neq(tx, r = digits(&byte, value, tx))) {
    if (byte) {
      return r;
    }
  }
  return tx;
}

Txpos *numbers_int(char **value, Txpos *tx) {
  Txpos *r;
  bool byte;
  if (txpos_neq(tx, r = digits(&byte, value, tx))) {
    if (!byte) {
      return r;
    }
  }
  return tx;
}

Txpos *numbers_float(char **value, Txpos *tx) {
  Txpos *start = tx;

  Buf *bf = buf_new();
  Txpos *r;
  char *v;
  bool int_empty = true;
  if (txpos_neq(tx, r = ddigits0(&v, tx))) {
    buf_add(bf, v);
    int_empty= false;
    tx = r;
  } else {
    buf_cadd(bf, '0');
  }

  char ch = *txpos_start(tx);
  if (ch != '.' && ch != 'e' && ch != 'E') {
    return start;
  }

  bool decimal_empty = true;
  if (txpos_neq(tx, r = token_cconst0(tx, '.'))) {
    buf_cadd(bf, '.');
    tx = r;

    if (txpos_neq(tx, r = ddigits00(&v, tx))) {
      buf_add(bf, v);
      decimal_empty = false;
      tx = r;
    } else {
      buf_cadd(bf, '0');
    }
  }

  if (int_empty && decimal_empty) {
    return start;
  }

  if (txpos_neq(tx, r = token_csplit0(&ch, tx, "eE"))) {
    buf_cadd(bf, ch);
    tx = r;

    if (txpos_neq(tx, r = token_csplit0(&ch, tx, "+-"))) {
      buf_cadd(bf, ch);
      tx = r;
    }

    if (txpos_eq(tx, r = ddigits0(&v, tx)))
      TH(tx) "Expected an exponent value" _TH

    buf_add(bf, v);
    tx = r;
  }

  *value = buf_to_str(bf);
  return tx;
}
