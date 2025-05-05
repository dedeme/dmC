// Copyright 27-Jan-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cts.h"
#include "DEFS.h"
#include "kut/path.h"
#include "kut/sys.h"

char *cts_data_path (void) {
   return path_cat(sys_user_home(), ".dmCApp", cts_app_name, NULL);
 }

char *cts_compilation_path (void) {
  return path_cat(cts_data_path(), "compilationDb", NULL);
}

char *cts_c_built_path (void) {
  return path_cat(cts_data_path(), "built.c", NULL);
}

char *cts_h_built_path (void) {
  return path_cat(cts_data_path(), "built.h", NULL);
}

char *cts_o_built_path (void) {
  return path_cat(cts_data_path(), "built.o", NULL);
}

char *cts_a_built_path (void) {
  return path_cat(cts_data_path(), "libbuilt.a", NULL);
}

static int t = TRUE;
int *cts_true (void) {
  return &t;
}

static int f = FALSE;
int *cts_false (void) {
  return &f;
}
