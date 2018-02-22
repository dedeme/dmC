/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <unistd.h>
#include <dm.h>
#include "../include/compile.h"

void compile (int is_server) {
  char *CP_PATH = "/deme/haxe-3.4.0";
  char *CP = path_cat(CP_PATH, "haxe", NULL);

  char *tmp = realpath("./", NULL);
  char *root = str_copy(tmp);
  free(tmp);

  void build(char *f) {
    char *cmd = str_printf("%s %s", CP, f);
    system(cmd);
  }

  void compiled (char *d) {
    char *path = path_cat(root, d, NULL);

    chdir(path);
    char *f = "build.hxml";
    if (file_exists(f)) {
      printf("Compiling %s\n", d);
      build(f);
    }

    f = "buildS.hxml";
    if (file_exists(f)) {
      printf("Compiling %s\n", d);
      build(f);
    }

    tmp = realpath("./", NULL);
    char *abs = str_copy(tmp);
    free(tmp);
    Arr *ds = file_dir(abs);
    FNX(moved, char, newd) {
      if (file_is_directory(newd)) {
        char *name = path_name(newd);
        compiled(path_cat(d, name, NULL));
      }
    }_FN
    it_each(it_from(ds), moved);

  }

  compiled("src");
}
