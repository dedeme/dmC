// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Lexer entry

#ifndef LEXER_LEXER_H
  # define LEXER_LEXER_H

#include <dmc/all.h>
#include "Cpath.h"
#include "ast/Class.h"

/// lexer_run starts lexer
void lexer_run(Arr/*char*/ *paths, Cpath *main_file);

/// lexer_compile retun can be null
Class *lexer_compile(Cpath *cpath);

#endif
