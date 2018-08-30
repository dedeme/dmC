// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/Arr.h"
#include "dmc/DEFS.h"
#include "readers/head.h"
#include "readers/body.h"


Structure *structure_read(RW *rw) {
  Structure *st = MALLOC(Structure);

  st->head = head_read(rw);

  st->body = MALLOC(Body);
  st->body->ps = arr_new();
  st->body->vs = arr_new();
  st->body->fs = arr_new();
  if (rw_rc(rw)->indent) st->body = body_read(rw, st->head);

  return st;
}
