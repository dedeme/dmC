// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Attibute parser.

#ifndef PARSER_PATTRIBUTE_H
  # define PARSER_PATTRIBUTE_H

#include <dmc/all.h>
#include "ast/Class.h"
#include "ast/Sym.h"

///
Sym *pattribute(
  Class *c, char *id, Atype *generics, bool is_static, bool is_import
);

#endif


