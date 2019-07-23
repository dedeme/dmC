// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ctemplate.h"
#include "reader.h"
#include "writer.h"

static void help (void) {
  puts(
    "Use:\n"
    "  ctemplate [<dir> | help]\n"
    "For example:\n"
    "  ctemplate ./\n"
    "  ctemplate project\n"
    "  ctemplate help"
  );
}

static void process_dirs(char *include, char *src) {
  GC_NEW

  char *fsrc, *finclude;
  EACH(file_dir(gc, src), char, f)
    GCL_NEW

    fsrc = str_f(gcl, "%s/%s", src, f);
    if (file_is_directory(fsrc)) {
      process_dirs(str_f(gcl, "%s/%s", include, f), fsrc);
    } else if (str_ends(f, ".c")) {
      finclude = str_f(gcl, "%s/%s.h", include, str_left(gc, f, -2));
      // Arr[Tpl]
      Arr *tps = reader_read(gcl, fsrc);
      if (arr_size(tps)) {
        if (writer_mk_tmp(finclude, fsrc, tps)) {
          writer_copy_tmp(finclude, fsrc);
        }
      }
    }

    GCL_FREE
  _EACH

  GC_FREE
}

int main (int argc, char *argv[]) {
  if (argc != 2) {
    puts("'ctemplate' allows one and only one argument");
    help();
    return 1;
  }

  if (str_eq(argv[1], "help")) {
    help();
    return 0;
  }

  GC_NEW

  sys_init(gc, "ctemplate");

  char *include = path_cat(gc, argv[1], "include", NULL);
  char *src = path_cat(gc, argv[1], "src", NULL);

  if (!file_exists(include))
    EXC_IO(str_f(gc, "File '%s' not found", include))

  if (!file_exists(src))
    EXC_IO(str_f(gc, "File '%s' not found", src))

  process_dirs(include, src);

  GC_FREE
}
