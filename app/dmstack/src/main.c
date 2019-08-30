// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "dmc/List.h"
#include "Reader.h"
#include "Machine.h"

void main_process (char *program) {
  Reader *r = reader_new("*string*", program, 1, 0);

  machine_process(list_new(), reader_process(r));
}
