// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Stat.h"
#include "ast/Aastat.h"
#include "ast/Avalue.h"

/*.
struct: Stat
  type: enum Stat_t : _uint
  pos: Pos *: pos
  id: char *: _string
  values: Avalue *: avalue
  blocks: Aastat *: aastat
*/

/*.-.*/
struct stat_Stat {
  enum Stat_t type;
  Pos *pos;
  char *id;
  Avalue *values;
  Aastat *blocks;
};

Stat *stat_new(
  enum Stat_t type,
  Pos *pos,
  char *id,
  Avalue *values,
  Aastat *blocks
) {
  Stat *this = MALLOC(Stat);
  this->type = type;
  this->pos = pos;
  this->id = id;
  this->values = values;
  this->blocks = blocks;
  return this;
}

inline
enum Stat_t stat_type(Stat *this) {
  return this->type;
}

inline
Pos *stat_pos(Stat *this) {
  return this->pos;
}

inline
char *stat_id(Stat *this) {
  return this->id;
}

inline
Avalue *stat_values(Stat *this) {
  return this->values;
}

inline
Aastat *stat_blocks(Stat *this) {
  return this->blocks;
}

Json *stat_serialize(Stat *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->type);
  arr_add(serial, pos_serialize(this->pos));
  jarr_astring(serial, this->id);
  arr_add(serial, avalue_serialize(this->values));
  arr_add(serial, aastat_serialize(this->blocks));
  return json_warray(serial);
}

Stat *stat_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Stat *this = MALLOC(Stat);
  size_t i = 0;
  this->type = jarr_guint(serial, i++);
  this->pos = pos_restore(arr_get(serial, i++));
  this->id = jarr_gstring(serial, i++);
  this->values = avalue_restore(arr_get(serial, i++));
  this->blocks = aastat_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

Stat *stat_new_val(Pos *pos, char *id, Value *value) {
  Stat *this = stat_new(SVAL, pos, id, avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_var(Pos *pos, char *id, Value *value) {
  Stat *this = stat_new(SVAR, pos, id, avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_assign(
  Pos *pos, char *op, Value *left_value, Value *right_value
) {
  Stat *this = stat_new(SASSIGN, pos, op, avalue_new(), aastat_new());
  avalue_add(this->values, left_value);
  avalue_add(this->values, right_value);
  return this;
}

Stat *stat_new_fn(Pos *pos, Value *value) {
  Stat *this = stat_new(SFN, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

inline
Stat *stat_new_break(Pos *pos) {
  return stat_new(SBREAK, pos, "", avalue_new(), aastat_new());
}

inline
Stat *stat_new_continue(Pos *pos) {
  return stat_new(SCONTINUE, pos, "", avalue_new(), aastat_new());
}

Stat *stat_new_throw(Pos *pos, Value *value) {
  Stat *this = stat_new(STHROW, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_return(Pos *pos, Value *value) {
  Stat *this = stat_new(SRETURN, pos, "", avalue_new(), aastat_new());
  if (value) {
    avalue_add(this->values, value);
  }
  return this;
}

Stat *stat_new_block(Pos *pos, Astat *block) {
  Stat *this = stat_new(SBLOCK, pos, "", avalue_new(), aastat_new());
  aastat_add(this->blocks, block);
  return this;
}

Stat *stat_new_while(Pos *pos, Value *condition, Astat *block) {
  Stat *this = stat_new(SWHILE, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, condition);
  aastat_add(this->blocks, block);
  return this;
}

inline
Stat *stat_new_if(Pos *pos, Avalue *values, Aastat *blocks) {
  return stat_new(SIF, pos, "", values, blocks);
}

inline
Stat *stat_new_try(Pos *pos, char *var, Aastat *blocks) {
  return stat_new(STRY, pos, var, avalue_new(), blocks);
}

inline
Stat *stat_new_native(Pos *pos, char *text) {
  return stat_new( SNATIVE, pos, "", avalue_new(), aastat_new());
}
