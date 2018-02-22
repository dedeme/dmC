// Copyright 13-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "utils.h"
#include <dm.h>

static char *data_dir(Cgi *cgi) {
  char *data_dir = path_cat(cgi_home(cgi), "data", NULL);
  if (!file_exists(data_dir)) {
    file_mkdir(data_dir);
  }
  return data_dir;
}

static char *conf_path(Cgi *cgi) {
  return path_cat(data_dir(cgi), "conf.db", NULL);
}

static char *paths_path(Cgi *cgi) {
  return path_cat(data_dir(cgi), "paths.db", NULL);
}

CgiRp *send_conf(Cgi *cgi) {
  char *file = conf_path(cgi);
  if (!file_exists(file)) {
    Arr/*Json*/ *a = arr_new();
    arr_add(a, json_wstring("@"));
    arr_add(a, json_wstring("es"));
    arr_add(a, json_wbool(true));
    file_write(file, json_warray(a));
  }

  Map/*Json*/ *m = map_new();
  jmap_pstring(m, "conf", file_read(file));
  return cgi_ok(cgi, m);
}

CgiRp *set_conf(Cgi *cgi, Arr/*Json*/ *data) {
  file_write(conf_path(cgi), json_warray(data));

  return cgi_ok(cgi, map_new());
}

CgiRp *send_paths(Cgi *cgi) {
  char *file = paths_path(cgi);
  if (!file_exists(file)) {
    file_write(file, json_warray(arr_new()));
  }

  Arr/*Json*/ *js_source_paths = json_rarray(file_read(file));
  Arr/*Json*/ *js_target_paths = arr_new();
  EACH(js_source_paths, Json, js_row) {
    Arr/*Json*/ *row = json_rarray(js_row);
    char *path = json_rstring(arr_get(row, 1));
    Json *js = file_exists(path) ? json_wbool(true) : json_wbool(false);
    arr_add(row, js);

    arr_add(js_target_paths, json_warray(row));
  }_EACH

  Map/*Json*/ *m = map_new();
  jmap_parray(m, "paths", js_target_paths);
  return cgi_ok(cgi, m);
}

CgiRp *set_paths(Cgi *cgi, Arr/*Json*/ *data) {
  file_write(paths_path(cgi), json_warray(data));

  return cgi_ok(cgi, map_new());
}

static char *read_help(char *file) {
  LckFile *lck = file_ropen(file);

  bool long_help = false;
  int status = 0;
  char *l = NULL;
  char *help = "";
  for(;;) {
    if (status == 2) {
      int ix = str_index(l, "*/");
      if (ix == -1) {
        status = 1;
        continue;
      } else {
        l = str_sub_end(l, ix + 2);
        status = 0;
      }
    } else {
      l = file_read_line(lck);
      if (!*l) {
        break;
      }
    }

    l = str_trim(l);
    if (status == 1) {
      if (!*l) {
        continue;
      }
      int ix = str_index(l, "*/");
      if (ix == -1) {
        continue;
      } else {
        status = 2;
        continue;
      }
    } else if (status == 3) {
      if (!*l) {
        continue;
      }
      if (str_starts(l, "*")) {
        l = str_trim(str_sub_end(l, 1));
      }
      help = l;
      break;
    } else {
      if (str_starts(l, "///")) {
        help = str_sub_end(l, 3);
        break;
      }
      if (str_starts(l, "/**")) {
        l = str_sub_end(l, 3);
        if (!*l) {
          status = 3;
          continue;
        }
        help = l;
        long_help = true;
        break;
      }
      if (!*l || str_starts(l, "//")) {
        continue;
      }
      if (!*l || str_starts(l, "/*")) {
        l = str_sub_end(l, 2);
        status = 2;
        continue;
      }
      help = "";
      break;
    }
  }

  if (long_help) {
    int ix = str_index(help, "*/");
    if (ix != -1) {
      help = str_trim(str_sub(help, 0, ix));
    }
  }

  int ix = str_cindex(help, '.');
  if (ix != -1) {
    help = str_trim(str_sub(help, 0, ix + 1));
  }

  file_close(lck);
  return help;
}

static Arr/*Json*/ *read_tree(char *id, char *dir) {
  Arr/*Json*/ *elements = arr_new();
  Arr/*char*/ *files = file_dir(dir);
  EACH(files, char, file) {
    char *name = path_name(file);
    if (file_is_directory(file)) {
      arr_add(elements, json_warray(read_tree(name, file)));
    } else if (str_ends(name, ".js")) {
      Arr/*Json*/ *sub_tree = arr_new();
      arr_add(sub_tree, json_wstring(name));
      arr_add(sub_tree, json_wstring(read_help(file)));
      arr_add(sub_tree, json_wnull());

      arr_add(elements, json_warray(sub_tree));
    }
  }_EACH

  Arr/*Json*/ *r = arr_new();
  arr_add(r, json_wstring(id));
  arr_add(r, json_wnull());
  arr_add(r, json_warray(elements));
  return r;
}

CgiRp *send_index_tree(Cgi *cgi, char *path) {
  Arr/*Json*/ *tree = NULL;
	if (file_is_directory(path)) {
		tree = read_tree("root", path);
	} else {
    tree = arr_new();
    arr_add(tree, json_wstring("root"));
    arr_add(tree, json_wnull());
    arr_add(tree, json_wnull());
	}

  Map/*Json*/ *m = map_new();
  jmap_parray(m, "tree", tree);
  return cgi_ok(cgi, m);
}

CgiRp *send_file(Cgi *cgi, char *path) {
  char *tx = file_exists(path) ? file_read(path) : "";

  Map/*Json*/ *m = map_new();
  jmap_pstring(m, "text", tx);
  return cgi_ok(cgi, m);
}
