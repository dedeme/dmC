// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/TpStrStr.h"

/*.
struct: Tp_str_str
  s1: char *: _string
  s2: char *: _string
*/

/*.-.*/
struct tp_str_str_Tp_str_str {
  char *s1;
  char *s2;
};

Tp_str_str *tp_str_str_new(char *s1, char *s2) {
  Tp_str_str *this = MALLOC(Tp_str_str);
  this->s1 = s1;
  this->s2 = s2;
  return this;
}

inline
char *tp_str_str_s1(Tp_str_str *this) {
  return this->s1;
}

inline
char *tp_str_str_s2(Tp_str_str *this) {
  return this->s2;
}

Json *tp_str_str_serialize(Tp_str_str *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_astring(serial, this->s1);
  jarr_astring(serial, this->s2);
  return json_warray(serial);
}

Tp_str_str *tp_str_str_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Tp_str_str *this = MALLOC(Tp_str_str);
  size_t i = 0;
  this->s1 = jarr_gstring(serial, i++);
  this->s2 = jarr_gstring(serial, i++);
  return this;
}
/*.-.*/
