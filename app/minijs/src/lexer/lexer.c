// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/lexer.h"
#include <sys/stat.h>
#include "Cpath.h"
#include "lexer/Txpos.h"
#include "lexer/token.h"
#include "lexer/block.h"

static void clean_lexer_dir(char *dir) {
  char *paths = path_cat(dir, "paths", NULL);
  file_mkdir(paths);
  int len = strlen(paths);

  void clean(char *clean_dir) {
    EACH(file_dir(clean_dir), char, path) {
      if (file_is_directory(path)) {
        if (!file_is_directory(str_sub_end(path, len))) {
          file_del(path);
        } else {
          clean_lexer_dir(path);
        }
      }
    }_EACH
  }

  char *counter = path_cat(dir, "counter.txt", NULL);
  if (!file_exists(counter)) {
    file_write(counter, "1");
  }

  int count = atoi(file_read(counter));
  if (count % 100 == 0) {
    clean(dir);
    count = 0;
  }
  file_write(counter, str_printf("%d", ++count));
}

static Class *direct_compile(Cpath *cfile) {
  char *f = cpath_file(cfile);
  if (!file_exists(f))
    THROW "File '%s' not found", f _THROW

  char *of = cpath_lib(cfile);
  if (!file_exists(of)) {
    return NULL;
  }

  size_t tf = file_info(f)->st_mtime;
  size_t tof = file_info(of)->st_mtime;

  if (tf > tof) {
    return NULL;
  }

  return class_restore(file_read(of));
}

Class *lexer_compile(Cpath *cpath) {
  Class *c = direct_compile(cpath);
  if (c) {
    return c;
  }

  char *f = cpath_file(cpath);
  char *of = cpath_lib(cpath);
  char *ftx = file_read(f);
  Txpos *tx = txpos_new(cpath, ftx, ftx, ftx + strlen(ftx), 1, 0);
  tx = token_blanks(tx);
  TRY {
    c = block_top_class(tx, cpath);
    char *parent = path_parent(of);
    file_mkdir(parent);
    file_write(of, class_serialize(c));
  } CATCH(e) {
    if (e[strlen(e) - 1] == '\1')
      file_del(of);
    else
      THROW e _THROW
  }_TRY

  return c;
}

void lexer_run(Arr/*char*/ *paths, Cpath *main_path) {
  clean_lexer_dir(sys_home());
  Class *c = lexer_compile(main_path);

// ------------------------------
// Test if c has main
// ------------------------------
if (c)
puts(class_serialize(c));
else
puts("c is null");

}
