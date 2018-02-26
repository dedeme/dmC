// Copyright 23-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ctemplate.h"
#include <dm.h>
#include <ctype.h>
#include "data.h"
#include "RW.h"
#include "readers/template.h"
#include "writers/tmpc.h"
#include "writers/tmph.h"

// static char *START_MARK = "/*.+.";
// static char *MARK = "/*.-.*/";

static void help() {
  puts(
    "Use: ctemplate path\n"
    "Where 'path' is the comun parent of 'include' and 'src'"
  );
}

static void process_file(char *fh, char *fc) {
  char *tmpc = path_cat(sys_home(), "c", NULL);
  RW *rw = rw_new(fc, tmpc);

  Template *tp = NULL;
  TRY {
    tp =  template_read(rw);
  } CATCH (e) {
    rw_close(rw);
    THROW e _THROW
  }_TRY

  if (!tp) {
    rw_close(rw);
    return;
  }

  TRY {
    tmpc_write(tp, rw);
  } CATCH (e) {
    rw_close(rw);
    THROW e _THROW
  } _TRY
  rw_close(rw);

  char *tmph = path_cat(sys_home(), "h", NULL);
  rw = rw_new(fh, tmph);

  TRY {
    tmph_write(tp, rw);
  } CATCH (e) {
    rw_close(rw);
    THROW e _THROW
  } _TRY
  rw_close(rw);

  file_copy(tmpc, fc);
  file_copy(tmph, fh);
}

static void process_dir(char *include, char *src) {
  EACH(file_dir(src), char, f) {
    char *fh = path_cat(include, path_name(f), NULL);
    if (file_is_directory(f)) {
      if (!file_is_directory(fh))
        THROW "'%s' is not a directory", fh _THROW
      process_dir(fh, f);
    } else  {
      fh = str_printf("%s.h", str_sub(fh, 0, strlen(fh) - 2));

      TRY {
        if (!file_exists(fh))
          THROW "'%s' not found", fh _THROW
        process_file(fh, f);
      } CATCH (e) {
        puts(e);
      }_TRY
    }
  }_EACH
}

int main (int argc, char **argv) {
  sys_init("ctemplate");

  if (argc != 2) {
    help();
    return 0;
  }

  char *path = argv[1];

  if (!file_is_directory(path))
    THROW "'%s' is not a directory", path _THROW

  char *include = path_cat(path, "include", NULL);
  if (!file_is_directory(include))
    THROW "'%s' is not a directory", include _THROW

  char *src = path_cat(path, "src", NULL);
  if (!file_is_directory(src))
    THROW "'%s' is not a directory", src _THROW

  process_dir(include, src);

  return 0;
}
