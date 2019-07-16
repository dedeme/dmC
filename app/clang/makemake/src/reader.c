// Copyright 14-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"
#include <sys/stat.h>
#include "dmc/Iarr.h"
#include "DEFS.h"

int reader_check_directories (void) {
  if (!file_is_directory("bin")) {
    puts("Directory 'bin' is missing");
    return 1;
  }
  if (!file_is_directory("include")) {
    puts("Directory 'include' is missing");
    return 1;
  }
  if (!file_is_directory("src")) {
    puts("Directory 'src' is missing");
    return 1;
  }
  return 0;
}

int reader_check_libraries (Arr *libs) {
  EACH(libs, char, l)
    char *lib = str_f("lib/lib%s/lib%s.a", l, l);
    if (!file_exists(lib)) {
      printf("'%s' is missing\n", lib);
      return 1;
    }
    char *include = str_f("lib/lib%s/include", l);
    if (!file_is_directory(include)) {
      printf("Directory '%s' is missing\n", include);
      return 1;
    }
  _EACH
  return 0;
}

Arr *reader_dependencies (char *file) {
  // Arr[char]
  Arr *r = arr_new();

  // Path goes without extension
  void add_path (char *path) {
    int fn (char *p) { return str_eq(p, path); }
    if (!it_contains(arr_to_it(r), (FPRED)fn)) {
      arr_push(r, path);
    }
  }

  void add_line_dep (char *parent, char *l) {
    l = str_trim(l);
    if (str_starts(l, "#include ")) {
      l = str_ltrim(str_right(l, 9));
      int last = strlen(l) - 1;
      if (
        last > 3 &&
        ( (l[0] == '<' && l[last] == '>') ||
          (l[0] == '"' && l[last] == '"')
        )
      ) {
        l = str_sub(l, 1, last);
        last -= 2;
        if (l[0] == '/') return;
        if (l[last - 1] != '.' || l[last] != 'h') return;

        if (*parent) {
          char *fpath = str_f("include/%s/%s", parent, l);
          if (file_exists(fpath)) {
            add_path(str_f("%s/%s", parent, str_left(l, -2)));
            return;
          }
        }
        char *fpath = str_f("include/%s", l);
        if (file_exists(fpath)) {
          add_path(str_f("%s", str_left(l, -2)));
        }
      }
    }
  }

  void add_file_deps (char *parent, char *file) {
    FileLck *lck = file_ropen(file);
    char *l = file_read_line(lck);
    while (*l) {
      add_line_dep(parent, l);
      l = file_read_line(lck);
    }
    file_close(lck);
  }

  char *cfile = str_f("src/%s.c", file);
  if (file_exists(cfile)) {
    add_file_deps("", cfile);
  }

  char *hfile = str_f("include/%s.h", file);
  if (file_exists(hfile)) {
    add_file_deps(path_parent(file), hfile);
  }

  return r;
}

Opt *reader_check_changes (void) {
  // Arr[char]
  Arr *ls = str_csplit(file_read("Makefile"), '\n');
  // Map[int]
  Map *ios = map_new();
  char *prefix = str_cat(OBJ_DIR, "/", NULL);
  int len_prefix = strlen(prefix);
  EACH_IX(ls, char, l, ix)
    if (str_starts(l, prefix)) {
      int *pix = ATOMIC(sizeof(int));
      *pix = ix;
      map_put(ios, str_sub(l, len_prefix, str_cindex(l, ':') - 1), pix);
    }
  _EACH

  int check (Iarr *ixs, char *path) {
    int path_len1 = strlen(path) - 1;
    char *cpath = str_f("%s/%s", "src", path);
    if (file_is_directory(cpath)) {
      EACH(file_dir(cpath), char, f)
        if (check (ixs, str_f("%s/%s", path, f))) return 1;
      _EACH
      return 0;
    } else if (path[path_len1] == 'c' && path[path_len1 - 1] == '.') {
      char *opath = str_new(path);
      opath[path_len1] = 'o';

      int fn (Kv *kv) { return str_eq(kv_key(kv), opath); }
      // Kv[int]
      Kv *kv = opt_nget(it_find(it_from((Arr *)ios), (FPRED)fn));
      if (kv) {
        char *hpath = str_new(path);
        hpath[path_len1] = 'h';
        struct stat *cstst = file_info(cpath);
        struct stat *ostst = file_info(str_f("%s/%s", OBJ_DIR, opath));
        struct stat *hstst = file_info(str_f("%s/%s", "include", hpath));
        if (
          cstst->st_mtime > ostst->st_mtime ||
          hstst->st_mtime > ostst->st_mtime
        ) {
          iarr_push(ixs, *((int *)kv_value(kv)));
        }
        return 0;
      }
      return 1;
    } else {
      return 0;
    }
  }

  Iarr *ixs = iarr_new();
  EACH(file_dir("src"), char, f)
    if (check(ixs, f)) return opt_empty();
  _EACH

  if (iarr_size(ixs)) {
    int changed = 0;
    IEACH(ixs, ix)
      char *line = arr_get(ls, ix);
      // Arr[char]
      Arr *parts = str_csplit_trim(line, ':');
      char *file = str_sub(arr_get(parts, 0), len_prefix, -2);
      // Arr[char]
      Arr *adeps = reader_dependencies(file);
      // Arr[char]
      Arr *hdeps = arr_new();
      EACH(adeps, char, d)
        arr_push(hdeps, str_f("include/%s.h src/%s.c", d, d));
      _EACH
      char *deps = str_cjoin(hdeps, ' ');

      if (!str_eq(deps, arr_get(parts, 1))) {
        arr_set(ls, ix, str_f("%s : %s", arr_get(parts, 0), deps));
        changed = 1;
      }
    _EACH

    if (changed) {
      return opt_new(str_cjoin(ls, '\n'));
    }
  }

  return opt_new("");
}

