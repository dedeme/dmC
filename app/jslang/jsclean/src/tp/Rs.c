// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tp/Rs.h"

/*.
-struct: Rs
  state: enum St
  read: char *
  rest: char *
*/
/*.-.*/
#include "dmc/ct/Ajson.h"

struct rs_Rs {
  enum St state;
  char *read;
  char *rest;
};

Rs *rs_new(enum St state, char *read, char *rest) {
  Rs *this = MALLOC(Rs);
  this->state = state;
  XNULL(read)
  this->read = read;
  XNULL(rest)
  this->rest = rest;
  return this;
}

enum St rs_state(Rs *this) {
  XNULL(this)
  return this->state;
}

char *rs_read(Rs *this) {
  XNULL(this)
  return this->read;
}

char *rs_rest(Rs *this) {
  XNULL(this)
  return this->rest;
}
/*.-.*/
