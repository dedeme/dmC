// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/lexer.h"

static void clean_lexer_dir(char *dir, int len) {
  EACH(file_dir(dir), char, path) {
    if (file_is_directory(path)) {
      if (!file_is_directory(str_sub_end(path, len))) {
        file_del(path);
      } else {
        clean_lexer_dir(path, len);
      }
    }
  }_EACH
}

char *lexer_run(Arr/*char*/ *paths, char *main_file) {
  clean_lexer_dir(sys_home(), strlen(sys_home()));

  if (*main_file != '/') {
    main_file = path_cat(file_cwd(), main_file, NULL);
  }

  char *parent = str_sub_end(path_parent(main_file), 1);
  char *file = path_name(main_file);

  char *target = path_cat(sys_home(), parent, NULL);
  file_mkdir(target);

  return target;
}
