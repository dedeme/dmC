// Copyright 02-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for classes header

#ifndef LEXER_HEAD_H
  #define LEXER_HEAD_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "ast/Import.h"
#include "ast/Implement.h"

/// head_private does not fail.
Txpos *head_local(Txpos *tx);

///
Txpos *head_generics(Arr/*char*/ **generics, Txpos *tx);

///
Txpos *head_imports(Arr/*Import*/ **imports, Txpos *tx);

///
Txpos *head_extends(Implement **export, Txpos *tx);

///
Txpos *head_implements(Arr/*Implement*/ **implements, Txpos *tx);

#endif
