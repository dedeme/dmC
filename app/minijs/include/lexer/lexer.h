// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Lexer entry

#ifndef LEXER_LEXER_H
  # define LEXER_LEXER_H

#include <dmc/all.h>
#include "Cpath.h"
#include "ast/Class.h"

/// lexer_run clean unused library files
void lexer_init();

/// lexer_compile throws an exception if it fails
Class *lexer_compile(Cpath *cpath);

#endif
