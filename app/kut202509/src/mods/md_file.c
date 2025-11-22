// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_file.h"
#include "kut/file.h"
#include "kut/path.h"
#include "exp.h"
#include "obj.h"
#include "runner/fail.h"

void fcopy (char *source, char *target) {
  if (file_is_directory(source)) {
    if (file_exists(target) && !file_is_directory(target))
      EXC_KUT(str_f("'%s' is not a directory", target));

    char *tdir = path_cat(target, path_base(source), NULL);
    file_del(tdir);
    file_mkdir(tdir);
    EACH(file_dir(source), char, fname) {
      fcopy(path_cat(source, fname, NULL), tdir);
    }_EACH
    return;
  }
  if (file_is_directory(target))
    target = path_cat(target, path_base(source), NULL);

  file_copy(source, target);
}

// -----------------------------------------------------------------------------

// \s -> <file>
static Exp *aopen (Arr *exps) {
  CHECK_PARS ("file.aopen", 1, exps);
  return obj_file(file_aopen(exp_get_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *base (Arr *exps) {
  CHECK_PARS ("file.base", 1, exps);
  return exp_string(path_base(exp_get_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *canonical (Arr *exps) {
  CHECK_PARS ("file.canonical", 1, exps);
  char *path = exp_get_string(arr_get(exps, 0));
  char *r = opt_get(path_canonical(path));
  if (!r)
    EXC_KUT(str_f("'%s' can not be canonicalized", path));
  return exp_string(r);
}

// \[s...] -> s
static Exp *cat (Arr *exps) {
  CHECK_PARS ("file.cat", 1, exps);

  //<char>
  Arr *a = arr_map(exp_get_array(arr_get(exps, 0)), (FMAP)exp_get_string);
  if (arr_size(a) == 0) arr_push(a, "./");
  else if (!*(char *)*arr_begin(a)) *arr_begin(a) = "."; // First entry == ""
  return exp_string(path_clean(arr_cjoin(a, '/')));
}

// \s -> ()
static Exp *cd (Arr *exps) {
  CHECK_PARS ("file.cd", 1, exps);
  file_cd(exp_get_string(arr_get(exps, 0)));
  return exp_empty();
}

// \s -> s
static Exp *clean (Arr *exps) {
  CHECK_PARS ("file.clean", 1, exps);
  return exp_string(path_clean(exp_get_string(arr_get(exps, 0))));
}

// \s -> ()
static Exp *close (Arr *exps) {
  CHECK_PARS ("file.close", 1, exps);
  file_close(obj_get_file(arr_get(exps, 0)));
  return exp_empty();
}

// \s, s -> ()
static Exp *copy (Arr *exps) {
  CHECK_PARS ("file.copy", 2, exps);
  fcopy(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}

// \s -> ()
static Exp *del (Arr *exps) {
  CHECK_PARS ("file.del", 1, exps);
  file_del(exp_get_string(arr_get(exps, 0)));
  return exp_empty();
}

// \s -> [s...]
static Exp *dir (Arr *exps) {
  CHECK_PARS ("file.dir", 1, exps);
  return exp_array(
    arr_map(file_dir(exp_get_string(arr_get(exps, 0))), (FMAP)exp_string)
  );
}

// \s -> b
static Exp *exists (Arr *exps) {
  CHECK_PARS ("file.exists", 1, exps);
  return exp_bool(file_exists(exp_get_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *extension (Arr *exps) {
  CHECK_PARS ("file.extension", 1, exps);
  return exp_string(path_extension(exp_get_string(arr_get(exps, 0))));
}

// \s -> b
static Exp *is_directory (Arr *exps) {
  CHECK_PARS ("file.isDirectory", 1, exps);
  return exp_bool(file_is_directory(exp_get_string(arr_get(exps, 0))));
}

// \s -> b
static Exp *is_link (Arr *exps) {
  CHECK_PARS ("file.isLink", 1, exps);
  return exp_bool(file_is_link(exp_get_string(arr_get(exps, 0))));
}

// \s -> b
static Exp *is_regular (Arr *exps) {
  CHECK_PARS ("file.isRegular", 1, exps);
  return exp_bool(file_is_regular(exp_get_string(arr_get(exps, 0))));
}

// \s -> ()
static Exp *mkdir (Arr *exps) {
  CHECK_PARS ("file.mkdir", 1, exps);
  file_mkdir(exp_get_string(arr_get(exps, 0)));
  return exp_empty();
}

// \s, s -> ()
static Exp *mklink (Arr *exps) {
  CHECK_PARS ("file.mklink", 2, exps);
  file_link(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}

// \s -> s
static Exp *parent (Arr *exps) {
  CHECK_PARS ("file.parent", 1, exps);
  return exp_string(path_parent(exp_get_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *read (Arr *exps) {
  CHECK_PARS ("file.read", 1, exps);
  return exp_string(file_read(exp_get_string(arr_get(exps, 0))));
}

// \<file>, i -> <bytes>
static Exp *read_bin (Arr *exps) {
  CHECK_PARS ("file.readBin", 2, exps);
  return obj_bytes(file_read_bin_buf(
    obj_get_file(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \<file> -> ([s] | [])
static Exp *read_line (Arr *exps) {
  CHECK_PARS ("file.readLine", 1, exps);
  char *l = file_read_line(obj_get_file(arr_get(exps, 0)));
  if (*l) {
    if (l[strlen(l) - 1] == '\n') l = str_left(l, -1);
    return exp_array(arr_new_from(exp_string(l), NULL));
  }
  return exp_array(arr_new());
}

// \s, s -> ()
static Exp *frename (Arr *exps) {
  CHECK_PARS ("file.rename", 2, exps);
  file_rename(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}

// \s -> <file>
static Exp *ropen (Arr *exps) {
  CHECK_PARS ("file.ropen", 1, exps);
  return obj_file(file_ropen(exp_get_string(arr_get(exps, 0))));
}

// \s -> i
static Exp *size (Arr *exps) {
  CHECK_PARS ("file.size", 1, exps);
  return exp_int(file_size(exp_get_string(arr_get(exps, 0))));
}

// \s -> i
static Exp *tm (Arr *exps) {
  CHECK_PARS ("file.tm", 1, exps);
  return exp_int(file_modified(exp_get_string(arr_get(exps, 0))));
}

// \s, s -> s
static Exp *tmp (Arr *exps) {
  CHECK_PARS ("file.tmp", 2, exps);
  return exp_string(file_tmp(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
}

// \ -> s
static Exp *wd (Arr *exps) {
  CHECK_PARS ("file.wd", 0, exps);
  return exp_string(file_wd());
}

// \s -> <file>
static Exp *wopen (Arr *exps) {
  CHECK_PARS ("file.wopen", 1, exps);
  return obj_file(file_wopen(exp_get_string(arr_get(exps, 0))));
}

// \s, s -> ()
static Exp *write (Arr *exps) {
  CHECK_PARS ("file.write", 2, exps);
  file_write(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}

// \<file>, <bytes> -> ()
static Exp *write_bin (Arr *exps) {
  CHECK_PARS ("file.writeBin", 2, exps);
  file_write_bin(
    obj_get_file(arr_get(exps, 0)),
    obj_get_bytes(arr_get(exps, 1))
  );
  return exp_empty();
}

// \<file>, s -> ()
static Exp *write_text (Arr *exps) {
  CHECK_PARS ("file.writeText", 2, exps);
  file_write_text(
    obj_get_file(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}


Bfunction md_file_get (char *fname) {
  if (!strcmp(fname, "aopen")) return aopen;
  if (!strcmp(fname, "base")) return base;
  if (!strcmp(fname, "canonical")) return canonical;
  if (!strcmp(fname, "cat")) return cat;
  if (!strcmp(fname, "cd")) return cd;
  if (!strcmp(fname, "clean")) return clean;
  if (!strcmp(fname, "close")) return close;
  if (!strcmp(fname, "copy")) return copy;
  if (!strcmp(fname, "del")) return del;
  if (!strcmp(fname, "dir")) return dir;
  if (!strcmp(fname, "exists")) return exists;
  if (!strcmp(fname, "extension")) return extension;
  if (!strcmp(fname, "isDirectory")) return is_directory;
  if (!strcmp(fname, "isLink")) return is_link;
  if (!strcmp(fname, "isRegular")) return is_regular;
  if (!strcmp(fname, "mkdir")) return mkdir;
  if (!strcmp(fname, "mklink")) return mklink;
  if (!strcmp(fname, "parent")) return parent;
  if (!strcmp(fname, "read")) return read;
  if (!strcmp(fname, "readBin")) return read_bin;
  if (!strcmp(fname, "readLine")) return read_line;
  if (!strcmp(fname, "rename")) return frename;
  if (!strcmp(fname, "ropen")) return ropen;
  if (!strcmp(fname, "size")) return size;
  if (!strcmp(fname, "tm")) return tm;
  if (!strcmp(fname, "tmp")) return tmp;
  if (!strcmp(fname, "wd")) return wd;
  if (!strcmp(fname, "wopen")) return wopen;
  if (!strcmp(fname, "write")) return write;
  if (!strcmp(fname, "writeBin")) return write_bin;
  if (!strcmp(fname, "writeText")) return write_text;

  EXC_KUT(fail_bfunction("file", fname));
  return NULL; // Unreachable
}
