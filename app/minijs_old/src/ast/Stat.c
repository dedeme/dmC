// Copyright 11-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ct/Avalue.h"
#include "ct/Astat.h"
#include "ct/Aastat.h"
#include "ast/Stat.h"
#include "ast/Value.h"
#include "lexer/Pos.h"

/*.
struct: @Stat
  t: enum Stat_t: _int
  pos: Pos *: pos
  id: char *: _string
  values: Avalue *: __avalue - value
  blocks: Aastat *: __aastat - astat2
*/

/*.-.*/
struct stat_Stat {
  enum Stat_t t;
  Pos *pos;
  char *id;
  Avalue *values;
  Aastat *blocks;
};

Stat *_stat_new(
  enum Stat_t t,
  Pos *pos,
  char *id,
  Avalue *values,
  Aastat *blocks
) {
  Stat *this = MALLOC(Stat);
  this->t = t;
  XNULL(pos)
  this->pos = pos;
  XNULL(id)
  this->id = id;
  XNULL(values)
  this->values = values;
  XNULL(blocks)
  this->blocks = blocks;
  return this;
}

enum Stat_t stat_t(Stat *this) {
  XNULL(this)
  return this->t;
}

Pos *stat_pos(Stat *this) {
  XNULL(this)
  return this->pos;
}

char *stat_id(Stat *this) {
  XNULL(this)
  return this->id;
}

Avalue *stat_values(Stat *this) {
  XNULL(this)
  return this->values;
}

Aastat *stat_blocks(Stat *this) {
  XNULL(this)
  return this->blocks;
}

Ajson *stat_to_json(Stat *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_aint(serial, this->t);
  ajson_add(serial, json_warray(pos_to_json(this->pos)));
  jarr_astring(serial, this->id);
  ajson_add(serial, json_warray(avalue_to_json(this->values, value_to_json)));
  ajson_add(serial, json_warray(aastat_to_json(this->blocks, astat2_to_json)));
  return serial;
}

Stat *stat_from_json(Ajson *serial) {
  XNULL(serial)
  Stat *this = MALLOC(Stat);
  size_t i = 0;
  this->t = jarr_gint(serial, i++);
  this->pos = pos_from_json(json_rarray(ajson_get(serial, i++)));
  this->id = jarr_gstring(serial, i++);
  this->values = avalue_from_json(json_rarray(ajson_get(serial, i++)), value_from_json);
  this->blocks = aastat_from_json(json_rarray(ajson_get(serial, i++)), astat2_from_json);
  return this;
}
/*.-.*/

Stat *stat_new_val(Pos *pos, char *id, Value *value) {
  Stat *this =_stat_new(stat_VAL, pos, id, avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_var(Pos *pos, char *id, Value *value) {
  Stat *this = _stat_new(stat_VAR, pos, id, avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_assign(
  Pos *pos, char *op, Value *left_value, Value *right_value
) {
  Stat *this = _stat_new(stat_ASSIGN, pos, op, avalue_new(), aastat_new());
  avalue_add(this->values, left_value);
  avalue_add(this->values, right_value);
  return this;
}

Stat *stat_new_fn(Pos *pos, Value *value) {
  Stat *this = _stat_new(stat_FN, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

inline
Stat *stat_new_break(Pos *pos) {
  return _stat_new(stat_BREAK, pos, "", avalue_new(), aastat_new());
}

inline
Stat *stat_new_continue(Pos *pos) {
  return _stat_new(stat_CONTINUE, pos, "", avalue_new(), aastat_new());
}

Stat *stat_new_throw(Pos *pos, Value *value) {
  Stat *this = _stat_new(stat_THROW, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, value);
  return this;
}

Stat *stat_new_return(Pos *pos, Value *value) {
  Stat *this = _stat_new(stat_RETURN, pos, "", avalue_new(), aastat_new());
  if (value) {
    avalue_add(this->values, value);
  }
  return this;
}

Stat *stat_new_block(Pos *pos, Astat *block) {
  Stat *this = _stat_new(stat_BLOCK, pos, "", avalue_new(), aastat_new());
  aastat_add(this->blocks, block);
  return this;
}

Stat *stat_new_while(Pos *pos, Value *condition, Astat *block) {
  Stat *this = _stat_new(stat_WHILE, pos, "", avalue_new(), aastat_new());
  avalue_add(this->values, condition);
  aastat_add(this->blocks, block);
  return this;
}

inline
Stat *stat_new_for(Pos *pos, Avalue *values, Aastat *blocks) {
  return _stat_new(stat_FOR, pos, "", values, blocks);
}

inline
Stat *stat_new_if(Pos *pos, Avalue *values, Aastat *blocks) {
  return _stat_new(stat_IF, pos, "", values, blocks);
}

inline
Stat *stat_new_try(Pos *pos, char *var, Aastat *blocks) {
  return _stat_new(stat_TRY, pos, var, avalue_new(), blocks);
}

inline
Stat *stat_new_native(Pos *pos, char *text) {
  return _stat_new(stat_NATIVE, pos, "", avalue_new(), aastat_new());
}
