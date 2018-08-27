// Copyright 10-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ct/Asym.h"
#include "dmc/ct/Ichar.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "lexer/Tx.h"
#include "DEFS.h"

#define TY sym
#define FN sym
#include "dmc/tpl/tarr.c"
#undef TY
#undef FN

void asym_add_test(Asym *this, Tx *tx, char *id) {
  if (ichar_contains((Ichar *)asym_to_it(this), id))
    TH(tx) "Identifier '%s' is duplicate", id _TH
  asym_add(this, id);
}
