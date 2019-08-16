// Copyright 15-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writer.h"
#include "DEFS.h"

int writer_prune (void) {
  int prune (char *path) {
    char *opath = str_f("%s/%s", OBJ_DIR, path);
    if (file_is_directory(opath)) {
      char *cpath = str_f("%s/%s", "src", path);
      if (!file_exists(cpath)) {
        file_del(opath);
        return 1;
      }
      int r = 0;
      EACH(file_dir(opath), char, f)
        r = r || prune(str_f("%s/%s", path, f));
      _EACH
      return r;
    }
    char *cpath = str_f("%s/%s", "src", path);
    cpath[strlen(cpath) - 1] = 'c';
    if (!file_exists(cpath)) {
      file_del(opath);
      return 1;
    }
    return 0;
  }

  if (!file_is_directory(OBJ_DIR)) return 1;

  int r = 0;
  EACH(file_dir(OBJ_DIR), char, f)
    r = r | prune(f);
  _EACH

  return r;
}

// libs is Arr[char], os is Arr[char]
void writer_mkmake (
  char *prg, Arr *libs, char *main_source, Arr *os, char *partial
) {
  file_mkdir(AUX_DIR);
  file_mkdir(OBJ_DIR);

  EACH(os, char, f)
    file_mkdir(str_f("%s/%s", OBJ_DIR, path_parent(f)));
  _EACH

  Buf *bf = buf_new();

  // Arr[char]
  Arr *lib_defs = arr_new();
  EACH(libs, char, lib)
    arr_push(lib_defs, str_f(
      "LIB_PATH_%s = lib/lib%s\n"
      "LIB_INCLUDE_%s = $(LIB_PATH_%s)/include\n",
      lib, lib, lib, lib
    ));
  _EACH
  buf_add(bf, str_cjoin(lib_defs, '\n'));

  buf_add(bf, "\nCFLAGS = -Wall -rdynamic\n");

  buf_add(bf, str_f(
    "bin/%s : src/%s.c include/%s.h %s\n",
    prg, main_source, main_source, LIB_FILE
  ));
  buf_add(bf, str_f(
    "\tgcc $(CFLAGS) src/%s.c -o bin/%s \\\n", main_source, prg
  ));

  // Arr[char]
  Arr *includes = arr_new();
  EACH(libs, char, lib)
    arr_push(includes, str_f(" \\\n\t\t-I$(LIB_INCLUDE_%s)", lib));
  _EACH
  buf_add(bf, "\t\t-Iinclude");
  buf_add(bf, str_join(includes, ""));

  // Arr[char]
  Arr *ls = arr_new();
  EACH(libs, char, lib)
    arr_push(ls, str_f(" \\\n\t\t-L$(LIB_PATH_%s) -l%s", lib, lib));
  _EACH
  buf_add(bf, str_f(" \\\n\t\t-L%s -l%s", AUX_DIR, LIB_NAME));
  buf_add(bf, str_join(ls, ""));

  buf_add(bf, " \\\n\t\t-lgc -lpthread -lm\n\n");

  // Arr[char]
  Arr *osf = arr_new();
  EACH(os, char, f)
    arr_push(osf, str_f("%s/%s.o", OBJ_DIR, f));
  _EACH
  char *oss = str_cjoin(osf, ' ');

  buf_add(bf, str_f("%s : %s\n", LIB_FILE, oss));
  buf_add(bf, str_f("\tar rcs %s %s\n\n", LIB_FILE, oss));

  buf_add(bf, partial);

  file_write("Makefile", buf_str(bf));
}
