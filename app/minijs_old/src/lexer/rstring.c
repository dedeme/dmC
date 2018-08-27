// Copyright 14-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Buf.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rstring.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "ast/Value.h"
#include "ct/Avatt.h"
#include "DEFS.h"

static void end_control(Tx *tx, Tx *start) {
  if (!*tx_start(tx))
    TH(start) "Quotes not closed" _TH
}

static Tx *triple_quoted(Value **v, Tx *tx,Tx *start) {
  char null;
  REPEAT(4) {
    tx = lex_char(&null, tx);
  }_REPEAT

  char *r;
  tx = lex_native(&r, tx, "\n\"\"\"");
  char *p = r;
  while (*p == ' ') {
    ++p;
  }

  int margin = p - r;
  Buf *bf = buf_new();
  int cmargin = 0;
  for (;;) {
    if (cmargin++ == margin || *r > ' ' || !*r) {
      break;
    }
    ++r;
  }
  while (*r) {
    buf_cadd(bf, *r);
    if (*r == '\n') {
      ++r;
      cmargin = 0;
      for (;;) {
        if (cmargin++ == margin || *r > ' ' || !*r) {
          break;
        }
        ++r;
      }
    } else {
      ++r;
    }
  }
  *v = value_new_str(tx_pos(start), avatt_new(), buf_to_str(bf));
  return tx;
}

static Tx *quotes(Value **v, Tx *tx,Tx *start, char quote) {
  Tx *r;
  char ch;
  Buf *bf = buf_new();
  for (;;) {
    end_control(tx, start);

    if (tx_neq(tx, r = lex_cconst(tx, '\\'))) {
      tx = r;
      end_control(tx, start);
      tx = lex_char(&ch, tx);
      if (ch != quote && ch != '\\') {
        buf_cadd(bf, '\\');
      }
      buf_cadd(bf, ch);
      continue;
    }

    tx = lex_char(&ch, tx);
    if (ch == quote) {
      break;
    }

    buf_cadd(bf, ch);
  }

  if (quote == '\'') {
    char *c = buf_to_str(bf);
    if (!*c)
      TH(lex_char(&ch, start)) "Expected a character" _TH
    *v = value_new_char(tx_pos(start), avatt_new(), c);
  } else if (quote == '`') {
    *v = value_new_str2(tx_pos(start), avatt_new(), buf_to_str(bf));
  } else {
    *v = value_new_str(tx_pos(start), avatt_new(), buf_to_str(bf));
  }
  return tx;
}

Tx *rstring(Value **v, Tx *tx) {
  Tx *r;
  Tx *start = tx;

  if (tx_neq(tx, r = lex_const(tx, "\"\"\"\n"))) {
    return triple_quoted(v, tx, start);
  }

  char quote;
  if (tx_eq(tx, r = lex_csplit(&quote, tx, "\"`'"))) {
    return tx;
  }

  return quotes(v, r, start, quote);
}

