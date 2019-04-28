// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "error.h"

void error_puts(char *file, int nline, char *line, char *msg) {
  char *m = str_f_new("%s:%d:%s\n  %s", file, nline, line, msg);
  puts(m);
  free(m);
}
