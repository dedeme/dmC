// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lint.h"
#include "dmc/ext.h"

void lint_run(Ap *files) {
  if (ap_size(files) == 0) {
    return;
  }

  void *map(void *p) {
    return p_absolute((P *)p);
  }
  Ichar *abs = (Ichar *)it_map(arr_to_it((Arr *)files), map);

  char *params = str_cjoin(abs, ' ');
  Achar *r = sys_cmd(str_printf("eslint %s", params));
  EACH(r, char, l) {
    puts(l);
  }_EACH
}
