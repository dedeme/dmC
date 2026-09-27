// Copyright 17-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/ogrouping.h"
#include "data/tp.h"

Arr *ogrouping_run (Arr *deps) {
  // Pushes e in a if it is not duplicated.
  // a is Arr<char>
  void push_if (Arr *a, char *e) {
    EACH(a, char, ea) {
      if (!strcmp(ea, e)) return;
    }_EACH
    arr_push(a, e);
  }

  // Array of elements and its forced_compilations.
  // Arr<Tp<char, Arr<char>>. (Arr<Tp<element, forced_compilations>>)
  Arr *r = arr_new();

  // Finds 'r' entry of 'e', and 'push_if' 'main' in its 'forced_compilations'
  // group.
  void add_force_compilation (char *e, char *main) {
    EACH(r, Tp, tp) {
      if (!strcmp(tp->e1, e)) {
        push_if(tp->e2, main);
        break;
      }
    }_EACH
  };

  // Returns the 'forced_compilations' group of 'e' in 'r'.
  // Returns Arr<char>
  Arr *get_forced_compilations (char *e) {
    EACH(r, Tp, tp) {
      if (!strcmp(tp->e1, e)) return tp->e2;
    }_EACH
    return arr_new();
  }

  EACH(deps, Tp, tp) {
    arr_push(r, tp_mk(tp->e1, arr_new_from(tp->e1, NULL)));
  }_EACH

  EACH(deps, Tp, tp) {
    char *main = tp->e1;
      EACH(tp->e2, char, d) {
        add_force_compilation(d, main);
      }_EACH
  }_EACH

  EACH(r, Tp, tp) {
    Arr *forced_compilations = tp->e2;
    int ix = 0;
    for(;;) {
      if (ix >= arr_size(forced_compilations)) break;
      // Arr<char>
      Arr *f_cs = get_forced_compilations(arr_get(forced_compilations, ix));
      EACH(f_cs, char, forced) {
        push_if(forced_compilations, forced);
      }_EACH
      ++ix;
    }
  }_EACH

  return r;
}

char *ogrouping_to_str (Arr *a) {
  Arr *bf = arr_new();
  EACH(a, Tp, g) {
    arr_push(bf, g->e1);
    EACH(g->e2, char, dep) {
      arr_push(bf, str_f("  %s", dep));
    }_EACH
  }_EACH

  return arr_cjoin(bf, '\n');
}
