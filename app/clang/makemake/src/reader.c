// Copyright 14-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"

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
