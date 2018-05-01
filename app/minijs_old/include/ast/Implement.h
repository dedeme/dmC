// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Extends/Implements entry
/// <h3>Generics notes:</h3>
/// Function 'implement_generics()' has the folowing complete definition:<br>
/// <tt><b>Arr[Tp_str_type] *import_renames(Import *this)</tt><br><br>

#ifndef AST_IMPLEMENT_H
  #define AST_IMPLEMENT_H

#include "dmc/all.h"
#include "Cpath.h"
#include "ast/TpStrType.h"
#include "lexer/Pos.h"

/*.-.*/

///
typedef struct implement_Implement Implement;

///
Implement *implement_new(Pos *pos, char *path, Arr *generics);

///
Pos *implement_pos(Implement *this);

///
char *implement_path(Implement *this);

///
Arr *implement_generics(Implement *this);

///
Json *implement_serialize(Implement *this);

///
Implement *implement_restore(Json *s);

/*.-.*/

#endif
