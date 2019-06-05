// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "error.h"

void error_puts(char *file, int nline, char *line, char *msg) {
  puts(str_f("%s:%d:%s\n  %s", file, nline, line, msg));
}
