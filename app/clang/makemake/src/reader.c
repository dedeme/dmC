// Copyright 06-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"

int reader_check_directories (void) {
  if (!file_exists("include")) {
    puts ("'include' directory is missing");
    return 0;
  }
  if (!file_exists("src")) {
    puts ("'src' directory is missing");
    return 0;
  }
  if (!file_exists("bin")) {
    puts ("'bin' directory is missing");
    return 0;
  }
  return 1;
}

int reader_check_libs (Arr *libs) {
  EACH(libs, char, l)
    char *include = str_f("lib/lib%s/include", l);
    if (!file_is_directory(include)) {
      printf("'%s' is not a directory\n", include);
      return 0;
    }
    char *lib = str_f("lib/lib%s/lib%s.a", l, l);
    if (!file_exists(lib)) {
      printf("'%s' is missing\n", lib);
      return 0;
    }
  _EACH
  return 1;
}

int reader_dependency (Node1 *parent, char *file, char *line, int nline) {
  int ix = str_index(line, "#include ");
  if (ix == -1) return 0;

  int ixq1 = -1;
  int ixq0 = str_cindex_from(line, '"', ix + 9);
  if (ixq0 == -1) {
    ixq0 = str_cindex_from(line, '<', ix + 9);
    ++ixq0;
    if (ixq0 == -1) return 0;
    ixq1 = str_cindex_from(line, '>', ixq0);
  } else {
    ++ixq0;
    ixq1 = str_cindex_from(line, '"', ixq0);
  }
  if (ixq1 == -1) return 0;

  char *dep = str_trim(str_sub(line, ixq0, ixq1));
  if (!*dep) return 0;

  char *pdep = path_parent(dep);
  if (!*pdep) {
    if (
      file_exists(str_f("include/%s.h", dep)) ||
      file_exists(str_f("src/%s.c", dep))
    ) {
      if (opt_is_empty(node1_parent(parent))) {
        return 0;
      } else {
        printf(
          "%s(Line: %d): Cyclic dependency '%s.(h|c)'\n",
          file, nline, dep
        );
        return 1;
      }
    } else {
      return 0;
    }
  }
  dep = pdep;

  if (str_eq(dep, node1_path(parent))) return 0;

  if (!file_is_directory(path_cat("include", dep, NULL))) return 0;

  Node1 *r = node1_new(opt_new(parent), dep, arr_new());
  EACH(node1_children(parent), Node1, ch)
    if (node1_eq(ch, r)) return 0;
  _EACH

  if (node1_cyclic(r)) {
    printf(
      "%s(Line: %d): Cyclic dependency '%s'\n",
      file, nline, dep
    );
    return 1;
  }

  arr_push(node1_children(parent), r);
  return 0;
}

int reader_dependencies (Node1 *parent) {
  int deps (char *root) {
    if (!file_is_directory(root)) return 0;
    int r = 0;
    EACH(file_dir(root), char, f)
      char *file = path_cat(root, f, NULL);
      if (file_is_directory(file)) continue;

      FileLck *lck = file_ropen(file);
      int nline = 1;
      char *l = file_read_line(lck);
      while (*l) {
        if (reader_dependency(parent, file, l, nline++)) break;
        l = file_read_line(lck);
      }
      file_close(lck);
      if (r) return r;
    _EACH
    return 0;
  }

  char *path = node1_path(parent);
  char *root = (*path) ? path_cat("include", path, NULL) : "include";
  if (deps(root)) return 1;
  root = (*path) ? path_cat("src", path, NULL) : "src";
  return deps(root);
}

Opt *reader_mk_tree (void) {
  int mk_tree (Node1 *n) {
    if (reader_dependencies(n)) return 1;
    EACH(node1_children(n), Node1, ch)
      if (mk_tree(ch)) return 1;
    _EACH
    return 0;
  }

  Node1 *n = node1_new(opt_empty(), "", arr_new());
  if (mk_tree(n)) return opt_empty();
  return opt_new(n);
}
