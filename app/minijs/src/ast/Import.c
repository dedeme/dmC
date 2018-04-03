// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Import.h"

/*.
struct: @Import
  pos: Pos *: pos
  itype: enum Itype_t: _int
  path: char *: _string
  id: char *: _string
  renames: Arr *: _array tp_str_str
*/

/*.-.*/
struct import_Import {
  Pos *pos;
  enum Itype_t itype;
  char *path;
  char *id;
  Arr *renames;
};

Import *_import_new(
  Pos *pos,
  enum Itype_t itype,
  char *path,
  char *id,
  Arr *renames
) {
  Import *this = MALLOC(Import);
  this->pos = pos;
  this->itype = itype;
  this->path = path;
  this->id = id;
  this->renames = renames;
  return this;
}

inline
Pos *import_pos(Import *this) {
  return this->pos;
}

inline
enum Itype_t import_itype(Import *this) {
  return this->itype;
}

inline
char *import_path(Import *this) {
  return this->path;
}

inline
char *import_id(Import *this) {
  return this->id;
}

inline
Arr *import_renames(Import *this) {
  return this->renames;
}

Json *import_serialize(Import *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_aint(serial, this->itype);
  jarr_astring(serial, this->path);
  jarr_astring(serial, this->id);
  jarr_aarray(serial, this->renames, (Json*(*)(void*)) tp_str_str_serialize);
  return json_warray(serial);
}

Import *import_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Import *this = MALLOC(Import);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->itype = jarr_gint(serial, i++);
  this->path = jarr_gstring(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->renames = jarr_garray(serial, i++, (void*(*)(Json*)) tp_str_str_restore);
  return this;
}
/*.-.*/

inline
Import *import_new_single(Pos *pos, char *path) {
  return _import_new(pos, ISINGLE, path, "", arr_new());
}

inline
Import *import_new_id(Pos *pos, char *path, char *id) {
  return _import_new(pos, IID, path, id, arr_new());
}

inline
Import *import_new_adds(Pos *pos, char *path, Arr/*Tp_str_str*/ *renames) {
  return _import_new(pos, IADDS, path, "", renames);
}

Import *import_new_subs(Pos *pos, char *path, Arr/*char*/ *ids) {
  Arr/*Tp_str_str*/ *renames = arr_new();
  EACH(ids, char, id) {
    arr_add(renames, tp_str_str_new(id, ""));
  }_EACH
  return _import_new(pos, ISUBS, path, "", renames);
}
