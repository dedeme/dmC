// Copyright 23-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "process.h"
#include "Def.h"
#include "Freader.h"
#include "reader.h"
#include "writer.h"

static void find_end_code(char **error_null, Freader *lck) {
  for (;;) {
    char *line = freader_line_new(lck);
    if (!*line) {
      free(line);
      freader_msg(error_null, lck, "End of file reached searching /*--*/");
      return;
    }
    char *tmp = line;
    line = str_trim_new(tmp);
    free(tmp);
    if (str_eq(line, "/*--*/")) {
      free(line);
      return;
    }
    free(line);
  }
}

static void process_c(
  char **error_null,
  // Arr[Def]
  Arr *defs,
  const char *ctmp,
  const char *cpath
) {
  *error_null = NULL;
  char *tmp = NULL;

  FileLck *tlck = file_wopen(ctmp);
  Freader *slck = freader_new(cpath);
  char *line = freader_line_new(slck);
  int defs_read = 0;
  int finished = 0;
  while (*line) {
    if (finished) {
      // Nothing
    } else if (defs_read) {
      char *l = str_new(line);
      tmp = l;
      l = str_trim_new(tmp);
      free(tmp);
      if (str_eq(l, "/*--*/")) {
        finished = 1;
        file_write_text(tlck, "/*--*/\n\n");
        find_end_code(error_null, slck);
        if (*error_null) {
          free(l);
          break;
        }
        writer_c(defs, tlck);
        free(line);
        line = str_new("/*--*/\n");
      }
      free(l);
    } else {
      char *l = str_new(line);
      tmp = l;
      l = str_trim_new(tmp);
      free(tmp);
      if (str_eq(l, "/*.")) {
        defs_read = 1;
        file_write_text(tlck, "/* .\n");  //"/* .\n");
        reader_run(error_null, defs, slck, tlck);
        if (*error_null) {
          free(l);
          break;
        }
        free(line);
        line = str_new("");
      }
      free(l);
    }
    file_write_text(tlck, line);
    free(line);
    line = freader_line_new(slck);
  }
  free(line);
  file_close(tlck);
  freader_close(slck);
}

static void process_h(
  char **error_null,
  // Arr[Def]
  Arr *defs,
  const char *htmp,
  const char *hpath
) {
  *error_null = NULL;
  if (!file_exists(hpath)) {
    *error_null = str_f_new("'%s' not found", hpath);
    return;
  }

  FileLck *tlck = file_wopen(htmp);
  Freader *slck = freader_new(hpath);
  char *line = freader_line_new(slck);
  int finished = 0;
  while (*line) {
    if (!finished) {
      char *l = str_new(line);
      char *tmp = l;
      l = str_trim_new(tmp);
      free(tmp);
      if (str_eq(l, "/*--*/")) {
        finished = 1;
        file_write_text(tlck, "/*--*/\n\n");
        find_end_code(error_null, slck);
        if (*error_null) {
          free(l);
          break;
        }
        writer_h(defs, tlck);
        free(line);
        line = str_new("/*--*/\n");
      }
      free(l);
    }
    file_write_text(tlck, line);
    free(line);
    line = freader_line_new(slck);
  }
  free(line);
  file_close(tlck);
  freader_close(slck);
}

static void process_src(
  const char *ctmp,
  const char *htmp,
  const char *src,
  const char *include,
  const char *dir
) {
  char *cdir = path_cat_new(src, dir, NULL);
  // Arr[char]
  Arr *fs = file_dir_new(cdir);
  EACH(fs, char, f) {
    char *rcpath = path_cat_new(dir, f, NULL);
    char *cpath = path_cat_new(src, rcpath, NULL);
    if (file_is_directory(cpath)) {
      process_src(ctmp, htmp, src, include, rcpath);
    } else if (str_ends(cpath, ".c")){
      // Arr[Def]
      Arr *defs = arr_new((FPROC)def_free);
      char *error_null;
      process_c(&error_null, defs, ctmp, cpath);
      if (error_null) {
        puts(error_null);
        free(error_null);
      } else if (arr_size(defs)) {
        char *rhpath = str_new(rcpath);
        char *tmp = rhpath;
        rhpath = str_left_new(tmp, strlen(tmp) - 1);
        free(tmp);
        tmp = rhpath;
        rhpath = str_cat_new(tmp, "h", NULL);
        free(tmp);
        char *hpath = path_cat_new(include, rhpath, NULL);
        process_h(&error_null, defs, htmp, hpath);
        if (error_null) {
          puts(error_null);
          free(error_null);
        } else {
          file_copy(ctmp, cpath);
          file_copy(htmp, hpath);
        }
        free(rhpath);
        free(hpath);
      }
      arr_free(defs);
    }
    free(rcpath);
    free(cpath);
  } _EACH
  free(cdir);
  arr_free(fs);
}

void process_root(const char *path) {
  char *ctmp = path_cat_new(sys_home(), "tmp.c", NULL);
  char *htmp = path_cat_new(sys_home(), "tmp.h", NULL);
  char *src_dir = path_cat_new(path, "src", NULL);
  char *include_dir = path_cat_new(path, "include", NULL);

  if (!file_exists(src_dir)) {
    printf("'%s' not found", src_dir);
  } else if (!file_is_directory(src_dir)) {
    printf("'%s' is not a directory", src_dir);
  } else if (!file_exists(include_dir)) {
    printf("'%s' not found", include_dir);
  } else if (!file_is_directory(include_dir)) {
    printf("'%s' is not a directory", include_dir);
  } else {
    process_src(ctmp, htmp, src_dir, include_dir, "./");
  }

  free(ctmp);
  free(htmp);
  free(src_dir);
  free(include_dir);
}
