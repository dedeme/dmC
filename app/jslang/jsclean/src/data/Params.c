// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/Params.h"

/* .
# Main parameters
Params
  # Arr[Char]
  roots: Arr - char
  target: char *
*/
/*--*/

struct Params_Params {
  Arr *roots;
  char *target;
};

Params *params_new (Arr *roots, char *target) {
  Params *this = MALLOC(Params);
  this->roots = roots;
  this->target = target;
  return this;
}

Arr *params_roots (Params *this) {
  return this->roots;
}

char *params_target (Params *this) {
  return this->target;
}

/*--*/

static char *help = ""
  "Use: jsclean roots target\n"
  "roots: Directory paths where find .js source files.\n"
  "target: Directory where cleaned files will be saved.\n"
  "For expample:\n"
  "jsclean src:lib www\n"
;

// Kv <Params>
Kv *params_read (int argc, char *argv[]) {
  if (argc != 3) {
    return kv_new(
      str_f("Wrong number of parameters (%d)\n%s", argc, help), ""
    );
  }

  // Arr[char]
  Arr *sources = str_csplit(argv[1], ':');

  // Arr[char]
  Arr *dup = tp_e1(arr_duplicates(sources, (FCMP)str_eq));

  if (arr_size(dup)) {
    char *fn (char *s) { return str_f("Source '%s' is duplicate", s); }
    return kv_new(str_f(
      "%s\n%s",
      str_cjoin(it_to(it_map(it_from(dup), (FCOPY)fn)), '\n'),
      help), ""
    );
  }
  return kv_new("", params_new(sources, argv[2]));
}
