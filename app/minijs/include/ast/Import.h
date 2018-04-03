// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import entry
/// <h3>Generics notes:</h3>
/// Function 'import renames()' has the folowing complete definition:<br>
/// <tt><b>Arr[Tp_str_str] *import_renames(Import *this)</tt><br><br>
/// When Import is of type 'ISUBS', 'tp_str_str_s1()' has the id value and
/// 'tp_str_str_s2()' has a blank ("") value.

#ifndef AST_IMPORT_H
  #define AST_IMPORT_H

#include "dmc/all.h"
#include "lexer/Pos.h"
#include "TpStrStr.h"
#include "DEFS.h"

/*.-.*/

///
typedef struct import_Import Import;

///
Pos *import_pos(Import *this);

///
enum Itype_t import_itype(Import *this);

///
char *import_path(Import *this);

///
char *import_id(Import *this);

///
Arr *import_renames(Import *this);

///
Json *import_serialize(Import *this);

///
Import *import_restore(Json *s);

/*.-.*/

///
Import *import_new_single(Pos *pos, char *path);

///
Import *import_new_id(Pos *pos, char *path, char *id);

///
Import *import_new_adds(Pos *pos, char *path, Arr/*Tp_str_str*/ *renames);

/// Values of tp_str_str_s2(renames) are NULL
Import *import_new_subs(Pos *pos, char *path, Arr/*char*/ *ids);

#endif
