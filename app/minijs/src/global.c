// Copyright 12-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "global.h"

static Arr/*char*/ *roots;
static Arr/*Final_class*/ *final_classes;
static Arr/*Class*/ *classes;

void global_init () {
  roots = arr_new();
  final_classes = arr_new();
  classes = arr_new();
}

inline
Arr/*char*/ *global_roots() {
  return roots;
}

inline
Arr/*Final_class*/ *global_final_classes() {
  return final_classes;
}

inline
Arr/*Class*/ *global_classes() {
  return classes;
}
