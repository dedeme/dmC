// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rstring.h"
#include "lexer/token.h"
#include "ast/Avatt.h"
#include "DEFS.h"

static void end_control(Tx *tx, Tx *start) {
  if (!*tx_start(tx))
    TH(start) "Quotes not closed" _TH
}

static Tx *triple_quoted(Value **v, Tx *tx,Tx *start) {
  char null;
  REPEAT(4) {
    tx = token_char(&null, tx);
  }_REPEAT

  char *r;
  tx = token_native(&r, tx, "\n\"\"\"");
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

    if (tx_neq(tx, r = token_cconst0(tx, '\\'))) {
      tx = r;
      end_control(tx, start);
      tx = token_char(&ch, tx);
      if (ch != quote && ch != '\\') {
        buf_cadd(bf, '\\');
      }
      buf_cadd(bf, ch);
      continue;
    }

    tx = token_char(&ch, tx);
    if (ch == quote) {
      tx = token_blanks(tx);
      break;
    }

    buf_cadd(bf, ch);
  }

  if (quote == '\'') {
    char *c = buf_to_str(bf);
    if (!*c)
      TH(token_char(&ch, start)) "Expected a character" _TH
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

  if (tx_neq(tx, r = token_const(tx, "\"\"\"\n"))) {
    return triple_quoted(v, tx, start);
  }

  char quote;
  if (tx_eq(tx, r = token_csplit0(&quote, tx, "\"`'"))) {
    return tx;
  }

  return quotes(v, r, start, quote);
}

