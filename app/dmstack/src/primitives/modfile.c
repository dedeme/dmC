// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfile.h"
#include <unistd.h>
#include <sys/stat.h>
#include "tk.h"
#include "fails.h"

static void cwd (Machine *m) {
  machine_push(m, token_new_string(file_cwd()));
}

static void cd (Machine *m) {
  char *path = tk_pop_string(m);
  file_cd(path);
}

static void smkdir (Machine *m) {
  char *path = tk_pop_string(m);
  file_mkdir(path);
}

static void dir (Machine *m) {
  char *path = tk_pop_string(m);
  Token *fn (char *s) { return token_new_string(s); }
  machine_push(m, token_new_list(arr_map(file_dir(path), (FCOPY)fn)));
}

static void isdirectory (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_new_int(file_is_directory(path)));
}

static void exists (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_new_int(file_exists(path)));
}

static void del (Machine *m) {
  char *path = tk_pop_string(m);
  file_del(path);
}

static void srename (Machine *m) {
  char *new = tk_pop_string(m);
  file_rename(tk_pop_string(m), new);
}

static void slink (Machine *m) {
  char *new = tk_pop_string(m);
  file_link(tk_pop_string(m), new);
}

static void copy (Machine *m) {
  char *new = tk_pop_string(m);
  file_copy(tk_pop_string(m), new);
}

static void tmpin (Machine *m) {
  char *prf = tk_pop_string(m);
  machine_push(m, token_new_string(file_tmp_in(tk_pop_string(m), prf)));
}

static void tmp (Machine *m) {
  machine_push(m, token_new_string(file_tmp(tk_pop_string(m))));
}

static void isregular (Machine *m) {
  char *p = tk_pop_string(m);
  int r = 0;
  if (file_exists(p)) {
    struct stat *st = file_info(p);
    r = S_ISREG(st->st_mode);
  }
  machine_push(m, token_new_int(r));
}

static void islink (Machine *m) {
  char *p = tk_pop_string(m);
  int r = 0;
  if (file_exists(p)) {
    struct stat *st = file_info(p);
    r = S_ISLNK(st->st_mode);
  }
  machine_push(m, token_new_int(r));
}

static void modified (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_new_int(file_modified(path)));
}

static void size (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_new_int(file_size(path)));
}

static void swrite (Machine *m) {
  char *text = tk_pop_string(m);
  file_write(tk_pop_string(m), text);
}

static void append (Machine *m) {
  char *text = tk_pop_string(m);
  file_append(tk_pop_string(m), text);
}

static void sread (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_new_string(file_read(path)));
}

static void aopen (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_from_pointer(symbol_FILE_, file_aopen(path)));
}

static void ropen (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_from_pointer(symbol_FILE_, file_ropen(path)));
}

static void wopen (Machine *m) {
  char *path = tk_pop_string(m);
  machine_push(m, token_from_pointer(symbol_FILE_, file_wopen(path)));
}

static void sclose (Machine *m) {
  file_close((FileLck *)tk_pop_native(m, symbol_FILE_));
}

static void readbin (Machine *m) {
  FileLck *f = tk_pop_native(m, symbol_FILE_);
  machine_push(m, token_from_pointer(symbol_BLOB_, file_read_bin(f)));
}

static void readline (Machine *m) {
  FileLck *f = tk_pop_native(m, symbol_FILE_);
  machine_push(m, token_new_string(file_read_line(f)));
}

static void writebin (Machine *m) {
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  FileLck *f = tk_pop_native(m, symbol_FILE_);
  file_write_bin(f, bs);
}

static void writetext (Machine *m) {
  char *s = tk_pop_string(m);
  FileLck *f = tk_pop_native(m, symbol_FILE_);
  file_write_text(f, s);
}

//
Pmodule *modfile_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("cwd", cwd); // [] - STRING
  add("cd", cd); // STRING - []
  add("mkdir", smkdir); // STRING - []
  add("dir", dir); // STRING - LIST<STRING>
  add("directory?", isdirectory); // STRING - INT

  add("exists?", exists);  // STRING - INT
  add("del", del); // STRING - []
  add("rename", srename); // [STRING, STRING] - []   (old, new) - []
  add("link", slink); // [STRING, STRING] - []   (old, new) - []
  add("copy", copy); // [STRING, STRING] - []   (source, target) - []
  add("tmpIn", tmpin); // [STRING, STRING] - STRING   (dir, prefix) - path
  add("tmp", tmp); // [STRING] - STRING   (prefix) - path

  add("regular?", isregular); // STRING - INT
  add("link?", islink); // STRING - INT
  add("modified", modified); // STRING - LONG
  add("size", size); // STRING - LONG

  add("write", swrite); // [STRING, STRING] - []   (path, text) - []
  add("append", append); // [STRING, STRING] - []   (path, text) - []
  add("read", sread); // STRING - STRING

  add("aopen", aopen); // STRING - CPOINTER
  add("ropen", ropen); // STRING - CPOINTER
  add("wopen", wopen); // STRING - CPOINTER
  add("close", sclose); // CPOINTER - []

  add("readBin", readbin); // CPOINTER - BLOB
  add("readLine", readline); // CPOINTER - STRING
  add("writeBin", writebin); // [CPOINTER, BLOB] - []
  add("writeText", writetext); // [CPOINTER, STRING] - []  (file, text) -[]

  return r;
}




