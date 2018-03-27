// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/strings.h"
#include "lexer/token.h"
#include "DEFS.h"

static Txpos *skip_double(Txpos *tx) {
  char *p = txpos_start(tx);
  char *end = txpos_end(tx);
  size_t nline = txpos_nline(tx);
  size_t nchar = txpos_nchar(tx);

  char *rune;
  int len;
  for (;;) {
    if (p == end)
      TH(tx) "End of 'txpos' reached" _TH

    if (*p == '"') {
      break;
    }

    switch (*p) {
    case '\\':
      ++p;
      if (*p == '\n') {
        ++p;
        ++nline;
        nchar = 0;
      } else {
        rune = str_next_rune(p);
        len = strlen(rune);
        if (!len)
          TH(tx) "End of 'txpos' reached" _TH
        p += len;
        nchar += 2;
      break;
    }
    case '\n':
      TH(tx) "End of line reached" _TH
    default:
      rune = str_next_rune(p);
      len = strlen(rune);
      if (!len)
        TH(tx) "End of 'txpos' reached" _TH
      p += len;
      ++nchar;
    }
  }

  return token_blanks(
    txpos_move(tx, p + 1, txpos_nline(tx), nchar + 1)
  );
}

static Txpos *skip_back(Txpos *tx) {
  char *p = txpos_start(tx);
  char *end = txpos_end(tx);
  size_t nline = txpos_nline(tx);
  size_t nchar = txpos_nchar(tx);

  char *rune;
  int len;
  for (;;) {
    if (p == end)
      TH(tx) "End of 'txpos' reached" _TH

    if (*p == '`') {
      break;
    }

    switch (*p) {
    case '\\':
      ++p;
      if (*p == '\n') {
        ++p;
        ++nline;
        nchar = 0;
      } else {
        rune = str_next_rune(p);
        len = strlen(rune);
        if (!len)
          TH(tx) "End of 'txpos' reached" _TH
        p += len;
        nchar += 2;
      }
      break;
    case '\n':
      ++p;
      ++nline;
      nchar = 0;
      break;
    default:
      rune = str_next_rune(p);
      len = strlen(rune);
      if (!len)
        TH(tx) "End of 'txpos' reached" _TH
      p += len;
      ++nchar;

    }
  }

  return token_blanks(
    txpos_move(tx, p + 1, txpos_nline(tx), nchar + 1)
  );
}

static Txpos *skip_single(Txpos *tx) {
  char *p = txpos_start(tx);
  char *end = txpos_end(tx);

  if (*p == '\n')
    TH(tx) "End of line reached" _TH

  char *rune = str_next_rune(p);
  int len = strlen(rune);
  p += strlen(rune);
  if (!len || p >= end)
    TH(tx) "End of 'txpos' reached" _TH

  if (*p != '\'')
    TH(tx) "Expected (') instead of (%s)", str_next_rune(p) _TH

  return token_blanks(
    txpos_move(tx, p + 1, txpos_nline(tx), txpos_nchar(tx) + 2)
  );
}

Txpos *strings_skip(Txpos *tx, char delimiter) {
  switch (delimiter) {
  case '"': return skip_double(tx);
  case '`': return skip_back(tx);
  case '\'': return skip_single(tx);
  default: THROW "'%c' unknown string delimiter", delimiter _THROW
  }
  return NULL;
}
