// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "params_reading.h"

static void help () {
  puts(
    "Use: jsclean roots target\n"
    "roots: Directory paths where find .js source files.\n"
    "target: Directory where cleaned files will be saved.\n"
    "For expample:\n"
    "jsclean src:lib www\n"
  );
}

Oparams *params_reading_run(int argc, char **argv) {
  if (argc != 3) {
    help ();
    return oparams_null();
  }

  Achar *sources = str_csplit(argv[1], ':');
  EACH(sources, char, s) {
    if (!file_is_directory(s)) {
      printf ("'%s' is not a directory\n", s);
      return oparams_null();
    }
  }_EACH

  char *target = argv[2];
  if (!file_is_directory(target)) {
    printf ("'%s' is not a directory\n", target);
    return oparams_null();
  }

  return oparams_new(params_new(sources, target));
}
