// Copyright 14-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Buf.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ast/Value.h"
#include "lexer/rnumber.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/Pos.h"
#include "DEFS.h"

#define BIN "01"
#define OCT "01234567"
#define DEC "0123456789"
#define DEC2 "0123456789_"
#define HEX "0123456789ABCDEFabcdef"

// Can not be any
static Tx *ddigits0(Tx *tx, Buf *bf, char *chs) {
  Tx *r;
  char d;
  while (tx_neq(tx, r = lex_csplit(&d, tx, chs))) {
    tx = r;
    if (d != '_') {
      buf_cadd(bf, d);
    }
  }
  return tx;
}

// Must be at least one
static Tx *ddigits(Tx *tx, Buf *bf, char *chs) {
  Tx *r;
  if (str_cindex(chs, *tx_start(tx)) == -1)
    TH(tx) "Expected a digit of '%s'", chs _TH

  tx = ddigits0(tx, bf, chs);
  if (tx_neq(tx, r = lex_cconst(tx, 't'))) {
    buf_cadd(bf, 't');
    return r;
  }
  return tx;
}

static Tx *rradix(Value **v, Tx *tx) {
  Tx *r;
  Tx *start = tx;
  Pos *pos = tx_pos(tx);

  if (tx_eq(tx, r = lex_cconst(tx, '0'))) {
    return tx;
  }
  tx = r;

  Buf *bf = buf_new();
  char d;
  if (tx_neq(tx, r = lex_cconst(tx, '.'))) {
    return start;
  } else if (tx_neq(tx, r = lex_cconst(tx, 'b'))) {
    buf_add(bf, "0b");
    tx = ddigits(r, bf, BIN);
  } else if (tx_neq(tx, r = lex_cconst(tx, 'x'))) {
    buf_add(bf, "0x");
    tx = ddigits(r, bf, HEX);
  } else if (tx_neq(tx, r = lex_cconst(tx, 't'))) {
    buf_add(bf, "0t");
    tx = r;
  } else if (tx_eq(tx, r = lex_csplit(&d, tx, OCT))) {
    buf_add(bf, "0");
  } else {
    buf_add(bf, "0o");
    tx = ddigits(tx, bf, OCT);
  }

  char *n = buf_to_str(bf);
  if (*(n + str_len(n) - 1) == 't') {
    *v = value_new_byte(pos, str_sub(n, 0, -1));
  } else {
    *v = value_new_int(pos, n);
  }

  return tx;
}

static Tx *rfloat(Tx *tx, Buf *bf) {
  Tx *r;
  if (tx_neq(tx, r = lex_cconst(tx, '.'))) {
    tx = r;
    buf_cadd(bf, '.');
    if (tx_eq(tx, r = ddigits0(tx, bf, DEC))) {
      buf_cadd(bf, '0');
    } else {
      tx = r;
    }
  }

  char d;
  if (tx_eq(tx, r = lex_csplit(&d, tx, "eE"))) {
    return tx;
  }
  tx = r;
  buf_cadd(bf, 'e');

  if (tx_neq(tx, r = lex_csplit(&d, tx, "-+"))) {
    tx = r;
    buf_cadd(bf, d);
  }

  if (tx_eq(tx, r = ddigits(tx, bf, DEC)))
    TH(tx) "Expected a digit" _TH

  return r;
}

Tx *rnumber(Value **v, Tx *tx) {
  Tx *r;

  if (tx_neq(tx, r = rradix(v, tx))) {
    return r;
  }

  Pos *pos = tx_pos(tx);
  Buf *bf = buf_new();
  char d;
  if (tx_eq(tx, r = lex_csplit(&d, tx, DEC))) {
    return tx;
  }

  tx = r;
  buf_cadd(bf, d);
  tx = ddigits0(tx, bf, DEC2);
  if (tx_neq(tx, r = lex_csplit(&d, tx, ".eE"))) { // Float
    tx = rfloat(tx, bf);
    *v = value_new_float(pos, buf_to_str(bf));
    return tx;
  }

  char *n = buf_to_str(bf);
  if (tx_neq(tx, r = lex_cconst(tx, 't'))) {
    *v = value_new_byte(pos, n);
  } else {
    *v = value_new_int(pos, n);
  }

  return tx;
}

#undef BIN
#undef OCT
#undef DEC
#undef DEC2
#undef HEX
