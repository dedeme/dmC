// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Value parser.

#ifndef PARSER_PVALUE_H
  # define PARSER_PVALUE_H

#include <dmc/all.h>
#include "ast/Class.h"
#include "ast/Value.h"
#include "ast/Lsymbol.h"

/// pvalue valids 'v' agains 'tp' using 'ok' which can be type_eq or type_child
void pvalue(
  Class *c, Lsymbol *ss, Type *tp, Value *v, bool(*ok)(Type *, Type *)
);

#endif


