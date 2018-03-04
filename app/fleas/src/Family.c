// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Family.h"
#include "dmc/all.h"

/*
struct: @Family
  opt: enum Family_option: _uint
  gen: Gen *: gen
*/

/*.-.*/
struct family_Family {
  enum Family_option opt;
  Gen *gen;
};

Family *_family_new(enum Family_option opt, Gen *gen) {
  Family *this = MALLOC(Family);
  this->opt = opt;
  this->gen = gen;
  return this;
}

inline
enum Family_option family_opt(Family *this) {
  return this->opt;
}

inline
Gen *family_gen(Family *this) {
  return this->gen;
}

Json *family_serialize(Family *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->opt);
  arr_add(serial, gen_serialize(this->gen));
  return json_warray(serial);
}

Family *family_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Family *this = MALLOC(Family);
  size_t i = 0;
  this->opt = jarr_guint(serial, i++);
  this->gen = gen_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

Family *family_new(enum Family_option opt) {
  Gen *gen = opt == FAMILIES_ALL ? gen_new(FAMILIES_END) : gen_new(1);
  return _family_new(opt, gen);
}

inline
bool family_eq(Family *this, Family *other) {
  return this->opt == other->opt &&
    (!this->gen || gen_actual(this->gen) == gen_actual(other->gen));
}

inline
Family *family_copy(Family *this) {
  return _family_new(this->opt, this->gen ? gen_copy(this->gen) : NULL);
}

inline
Family *family_mutate(Family *this) {
  return _family_new(this->opt, this->gen ? gen_mutate(this->gen) : NULL);
}

inline
uint family_max(Family *this) {
  if (!this->gen)
    THROW exc_null_pointer("this->gen") _THROW

  return gen_max(this->gen);
}

inline
uint family_actual(Family *this) {
  if (!this->gen)
    THROW exc_null_pointer("this->gen") _THROW
  return gen_actual(this->gen);
}

