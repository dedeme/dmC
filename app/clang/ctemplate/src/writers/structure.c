// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/structure.h"
#include "writers/struct.h"
#include "writers/typedef.h"
#include "writers/new.h"
#include "writers/funcs.h"
#include "writers/serial.h"
#include "writers/restore.h"

void structure_write(RW *rw, Structure *st) {
  struct_write(rw, st);
  typedef_write(rw, st);
  new_write(rw, st);
  funcs_write(rw, st);
  serial_write(rw, st);
  restore_write(rw, st);
}
