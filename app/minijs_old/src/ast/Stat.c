// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Stat.h"

static Json *statbl_serialize(Arr/*Stat*/ *stats) {
  if (!stats) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_aarray(serial, stats, (Json *(*)(void *))stat_serialize);
  return json_warray(serial);
}

static Arr/*Stat*/ *statbl_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  return jarr_garray(serial, 0, (void *(*)(Json *))stat_restore);
}

/*.
struct: @Stat
  pos: Pos *: pos
  stype: enum Stat_t: _uint
  ids: Arr *: _sarray
  values: Arr *: _array value
  dvalues: Arr *: _array dvalue
  blocks: Arr *: _array statbl
*/

/*.-.*/
struct stat_Stat {
  Pos *pos;
  enum Stat_t stype;
  Arr *ids;
  Arr *values;
  Arr *dvalues;
  Arr *blocks;
};

Stat *_stat_new(
  Pos *pos,
  enum Stat_t stype,
  Arr *ids,
  Arr *values,
  Arr *dvalues,
  Arr *blocks
) {
  Stat *this = MALLOC(Stat);
  this->pos = pos;
  this->stype = stype;
  this->ids = ids;
  this->values = values;
  this->dvalues = dvalues;
  this->blocks = blocks;
  return this;
}

inline
Pos *stat_pos(Stat *this) {
  return this->pos;
}

inline
enum Stat_t stat_stype(Stat *this) {
  return this->stype;
}

inline
Arr *stat_ids(Stat *this) {
  return this->ids;
}

inline
Arr *stat_values(Stat *this) {
  return this->values;
}

inline
Arr *stat_dvalues(Stat *this) {
  return this->dvalues;
}

inline
Arr *stat_blocks(Stat *this) {
  return this->blocks;
}

Json *stat_serialize(Stat *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_auint(serial, this->stype);
  jarr_aarray(serial, this->ids, (Json*(*)(void*))json_wstring);
  jarr_aarray(serial, this->values, (Json*(*)(void*)) value_serialize);
  jarr_aarray(serial, this->dvalues, (Json*(*)(void*)) dvalue_serialize);
  jarr_aarray(serial, this->blocks, (Json*(*)(void*)) statbl_serialize);
  return json_warray(serial);
}

Stat *stat_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Stat *this = MALLOC(Stat);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->stype = jarr_guint(serial, i++);
  this->ids = jarr_garray(serial, i++, (void*(*)(Json*))json_rstring);
  this->values = jarr_garray(serial, i++, (void*(*)(Json*)) value_restore);
  this->dvalues = jarr_garray(serial, i++, (void*(*)(Json*)) dvalue_restore);
  this->blocks = jarr_garray(serial, i++, (void*(*)(Json*)) statbl_restore);
  return this;
}
/*.-.*/

Stat *_new(Pos *pos, enum Stat_t stype) {
  return _stat_new(pos, stype, arr_new(), arr_new(), arr_new(), arr_new());
}

Stat *stat_new_val(Pos *pos, Dvalue *value) {
  Stat *this = _new(pos, SVAL);
  arr_add(this->dvalues, value);
  return this;
}

Stat *stat_new_var(Pos *pos, Dvalue *value) {
  Stat *this = _new(pos, SVAR);
  arr_add(this->dvalues, value);
  return this;
}

Stat *stat_new_fn(Pos *pos, Value *value) {
  Stat *this = _new(pos, SFN);
  arr_add(this->values, value);
  return this;
}

Stat *stat_new_inc(Pos *pos, Value *value) {
  Stat *this = _new(pos, SINC);
  arr_add(this->values, value);
  return this;
}

Stat *stat_new_assign(
  Pos *pos, char *op, Value *left_value, Value *right_value
) {
  Stat *this = _new(pos, SASSIGN);
  arr_add(this->ids, op);
  arr_add(this->values, left_value);
  arr_add(this->values, right_value);
  return this;
}

inline
Stat *stat_new_break(Pos *pos) {
  return _new(pos, SBREAK);
}

Stat *stat_new_continue(Pos *pos) {
  return _new(pos, SCONTINUE);
}

Stat *stat_new_throw(Pos *pos, Value *value) {
  Stat *this = _new(pos, STHROW);
  arr_add(this->values, value);
  return this;
}

Stat *stat_new_return(Pos *pos, Value *value) {
  Stat *this = _new(pos, SRETURN);
  if (value){
    arr_add(this->values, value);
  }
  return this;
}

Stat *stat_new_block(Pos *pos, Arr/*Stat*/ *block) {
  Stat *this = _new(pos, SBLOCK);
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_while(Pos *pos, Value *condition, Arr/*Stat*/ *block) {
  Stat *this = _new(pos, SWHILE);
  arr_add(this->values, condition);
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_do(Pos *pos, Value *condition, Arr/*Stat*/ *block) {
  Stat *this = _new(pos, SDO);
  arr_add(this->values, condition);
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_for0(
  Pos *pos, char *var, Arr/*Value*/ *values, Arr/*Stat*/ *block
) {
  Stat *this = _new(pos, SFOR0);
  arr_add(this->ids, var);
  this->values = values;
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_for(
  Pos *pos, char *var, Arr/*Value*/ *values, Arr/*Stat*/ *block
) {
  Stat *this = _new(pos, SFOR);
  arr_add(this->ids, var);
  this->values = values;
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_for_each(
  Pos *pos, char *var, Value *value, Arr/*Stat*/ *block
) {
  Stat *this = _new(pos, SFOR_EACH);
  arr_add(this->ids, var);
  arr_add(this->values, value);
  arr_add(this->blocks, block);
  return this;
}

Stat *stat_new_if(Pos *pos, Arr/*Value*/ *values, Arr/*Arr[Stat]*/ *blocks) {
  Stat *this = _new(pos, SIF);
  this->values = values;
  this->blocks = blocks;
  return this;
}

Stat *stat_new_try(Pos *pos, char *var, Arr/*Arr[Stat]*/ *blocks) {
  Stat *this = _new(pos, STRY);
  arr_add(this->ids, var);
  this->blocks = blocks;
  return this;
}

Stat *stat_new_native(Pos *pos, char *text) {
  Stat *this = _new(pos, SNATIVE);
  arr_add(this->ids, text);
  return this;
}
