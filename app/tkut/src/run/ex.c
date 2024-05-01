// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/js.h"
#include "run/ex.h"

static Ex *new (int nline, int nchar, ExT type, void *value) {
  Ex *this = MALLOC(Ex);
  this->nline = nline;
  this->nchar = nchar;
  this->type = type;
  this->value = value;
  return this;
}

Ex *ex_mk_string (int nline, int nchar, char *value) {
  return new(nline, nchar, EX_STRING, value);
}

char *ex_string (Ex *this);

Ex *exp_solve(RnCode *code, Ex *this);

char *ex_to_js (Ex *this) {
  char *value = NULL; // JSON value.
  switch (this->type) {
    case EX_STRING:
      value = js_ws(this->value);
      break;
  }
  return js_wa(arr_new_from(
    js_wi(this->nline),
    js_wi(this->nchar),
    js_wi(this->type),
    value,
    NULL
  ));
}

Ex *ex_from_js (char *js) {
  /// <char>
  char **a = (char **)arr_begin(js_ra(js));
  ExT type = js_ri(a[2]);
  char *vjs = a[3];
  void *value = NULL;
  switch (type) {
    case EX_STRING:
      value = js_rs(vjs);
      break;
  }
  return new (
    js_ri(a[0]),
    js_ri(a[1]),
    type,
    value
  );
}
