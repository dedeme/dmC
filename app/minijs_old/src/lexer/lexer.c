// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/lexer.h"
#include <sys/stat.h>
#include "Cpath.h"
#include "DEFS.h"
#include "lexer/Txpos.h"
#include "lexer/token.h"
#include "lexer/block.h"

static void clean_lexer_dir(char *dir) {
  char *paths = path_cat(dir, "paths", NULL);
  file_mkdir(paths);
  int len = strlen(paths);

  void clean(char *clean_dir) {
    Arr/*char*/ *sub = file_dir(clean_dir);
    if (!arr_size(sub)) {
      file_del(clean_dir);
      return;
    }
    EACH(sub, char, path) {
      char *pbase = str_ends(path, ".js") ? str_sub(path, 0, -3) : path;
      if (file_is_directory(path)) {
        clean(path);
      } else if (!file_exists(str_printf("%s.mini", str_sub_end(pbase, len)))) {
        file_del(path);
      }
    }_EACH
  }

  char *counter = path_cat(dir, "counter.txt", NULL);
  if (!file_exists(counter)) {
    file_write(counter, "1");
  }

  int count = atoi(file_read(counter));
  if (count % 100 == 0) {
    clean(paths);
    count = 0;
  }
  file_write(counter, str_printf("%d", ++count));
}

static Class *direct_compile(Cpath *cfile) {
  char *f = cpath_file(cfile);
  if (!file_exists(f))
    TH2(class_mk(cfile), pos_new(0,0)) "File '%s' not found", f _TH

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
    if (file_exists(of)) {
      file_del(of);
    }
    THROW e _THROW
  }_TRY

  return c;
}

inline
void lexer_init() {
  clean_lexer_dir(sys_home());
}
