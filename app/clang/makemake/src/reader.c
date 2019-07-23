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
    Gc *gc = gc_new();
    char *lib = str_f(gc, "lib/lib%s/lib%s.a", l, l);
    if (!file_exists(lib)) {
      printf("'%s' is missing\n", lib);
      return 1;
    }
    char *include = str_f(gc, "lib/lib%s/include", l);
    if (!file_is_directory(include)) {
      printf("Directory '%s' is missing\n", include);
      return 1;
    }
    gc_free(gc);
  _EACH
  return 0;
}

Arr *reader_dependencies (Gc *gc, char *file) {
  // Arr[char]
  Arr *r = arr_new(gc);

  // Path goes without extension
  void add_path (char *path) {
    Gc *gc = gc_new();
    int fn (char *p) { return str_eq(p, path); }
    if (!it_contains(arr_to_it(gc, r), (FPRED)fn)) {
      arr_push(r, path);
    }
    gc_free(gc);
  }

  void add_line_dep (char *parent, char *l) {
    Gc *gcl = gc_new();

    l = str_trim(gcl, l);
    if (str_starts(l, "#include ")) {
      l = str_ltrim(gcl, str_right(gcl, l, 9));
      int last = strlen(l) - 1;
      if (
        last > 3 &&
        ( (l[0] == '<' && l[last] == '>') ||
          (l[0] == '"' && l[last] == '"')
        )
      ) {
        l = str_sub(gcl, l, 1, last);
        last -= 2;
        if (l[0] == '/') return;
        if (l[last - 1] != '.' || l[last] != 'h') return;

        if (*parent) {
          char *fpath = str_f(gcl, "include/%s/%s", parent, l);
          if (file_exists(fpath)) {
            add_path(str_f(gc, "%s/%s", parent, str_left(gcl, l, -2)));
            return;
          }
        }
        char *fpath = str_f(gcl, "include/%s", l);
        if (file_exists(fpath)) {
          add_path(str_f(gc, "%s", str_left(gcl, l, -2)));
        }
      }
    }
    gc_free(gcl);
  }

  void add_file_deps (char *parent, char *file) {
    Gc *gc = gc_new();
    FileLck *lck = file_ropen(gc, file);
    char *l = file_read_line(gc, lck);
    while (*l) {
      add_line_dep(parent, l);
      l = file_read_line(gc, lck);
    }
    file_close(lck);
    gc_free(gc);
  }

  Gc *gcl = gc_new();
  char *cfile = str_f(gcl, "src/%s.c", file);
  if (file_exists(cfile)) {
    add_file_deps("", cfile);
  }

  char *hfile = str_f(gcl, "include/%s.h", file);
  if (file_exists(hfile)) {
    add_file_deps(path_parent(gc, file), hfile);
  }

  gc_free(gcl);
  return r;
}

Opt *reader_check_changes (Gc *gc) {
  Gc *gcl = gc_new();

  // Arr[char]
  Arr *ls = str_csplit(gcl, file_read(gcl, "Makefile"), '\n');
  // Map[int]
  Map *ios = map_new(gcl);
  char *prefix = str_cat(gc, OBJ_DIR, "/", NULL);
  int len_prefix = strlen(prefix);
  EACH_IX(ls, char, l, ix)
    if (str_starts(l, prefix)) {
      int *pix = gc_add(gcl, malloc(sizeof(int)));
      *pix = ix;
      map_put(ios, str_sub(gcl, l, len_prefix, str_cindex(l, ':') - 1), pix);
    }
  _EACH

  int check (Iarr *ixs, char *path) {
    Gc *gcf = gc_new();

    int path_len1 = strlen(path) - 1;
    char *cpath = str_f(gcf, "%s/%s", "src", path);
    if (file_is_directory(cpath)) {
      EACH(file_dir(gcf, cpath), char, f)
        if (check (ixs, str_f(gcf, "%s/%s", path, f))) {
          gc_free(gcf);
          return 1;
        }
      _EACH
      gc_free(gcf);
      return 0;
    } else if (path[path_len1] == 'c' && path[path_len1 - 1] == '.') {
      char *opath = str_new(gcf, path);
      opath[path_len1] = 'o';

      int fn (Kv *kv) { return str_eq(kv_key(kv), opath); }
      // Kv[int]
      Kv *kv = opt_nget(it_find(it_from(gcf, (Arr *)ios), (FPRED)fn));
      if (kv) {
        char *hpath = str_new(gcf, path);
        hpath[path_len1] = 'h';
        struct stat *cstst = file_info(gcf, cpath);
        struct stat *ostst = file_info(
          gcf, str_f(gcf, "%s/%s", OBJ_DIR, opath)
        );
        struct stat *hstst = file_info(
          gcf, str_f(gcf, "%s/%s", "include", hpath)
        );
        if (
          cstst->st_mtime > ostst->st_mtime ||
          hstst->st_mtime > ostst->st_mtime
        ) {
          iarr_push(ixs, *((int *)kv_value(kv)));
        }
        gc_free(gcf);
        return 0;
      }
      gc_free(gcf);
      return 1;
    } else {
      gc_free(gcf);
      return 0;
    }
  }

  Iarr *ixs = iarr_new(gcl);
  EACH(file_dir(gcl, "src"), char, f)
    if (check(ixs, f)) return opt_empty();
  _EACH

  if (iarr_size(ixs)) {
    int changed = 0;
    IEACH(ixs, ix)
      Gc *gcf = gc_new();

      char *line = arr_get(ls, ix);
      // Arr[char]
      Arr *parts = str_csplit_trim(gcf, line, ':');
      char *file = str_sub(gcf, arr_get(parts, 0), len_prefix, -2);
      // Arr[char]
      Arr *adeps = reader_dependencies(gcf, file);
      // Arr[char]
      Arr *hdeps = arr_new(gcf);
      EACH(adeps, char, d)
        arr_push(hdeps, str_f(gcf, "include/%s.h src/%s.c", d, d));
      _EACH
      char *deps = str_cjoin(gcf, hdeps, ' ');

      if (!str_eq(deps, arr_get(parts, 1))) {
        arr_set(ls, ix, str_f(gcl, "%s : %s", arr_get(parts, 0), deps));
        changed = 1;
      }

      gc_free(gcf);
    _EACH

    if (changed) {
      char *r = str_cjoin(gc, ls, '\n');
      gc_free(gcl);
      return opt_new(r);
    }
  }

  gc_free(gcl);
  return opt_new("");
}

