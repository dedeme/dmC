// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfile.h"
#include <unistd.h>
#include <sys/stat.h>
#include "Machine.h"
#include "fails.h"

static void cwd (Machine *m) {
  machine_push(m, token_new_string(0, file_cwd()));
}

static void cd (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  file_cd(path);
}

static void smkdir (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  file_mkdir(path);
}

static void dir (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  Token *fn (char *s) { return token_new_string(0, s); }
  machine_push(m, token_new_list(0, arr_map(file_dir(path), (FCOPY)fn)));
}

static void isdirectory (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_int(0, file_is_directory(path)));
}

static void exists (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_int(0, file_exists(path)));
}

static void del (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  file_del(path);
}

static void srename (Machine *m) {
  char *new = token_string(machine_pop_exc(m, token_STRING));
  file_rename(token_string(machine_pop_exc(m, token_STRING)), new);
}

static void slink (Machine *m) {
  char *new = token_string(machine_pop_exc(m, token_STRING));
  file_link(token_string(machine_pop_exc(m, token_STRING)), new);
}

static void copy (Machine *m) {
  char *new = token_string(machine_pop_exc(m, token_STRING));
  file_copy(token_string(machine_pop_exc(m, token_STRING)), new);
}

static void isregular (Machine *m) {
  struct stat *st = file_info(token_string(machine_pop_exc(m, token_STRING)));
  machine_push(m, token_new_int(0, S_ISREG(st->st_mode)));
}

static void islink (Machine *m) {
  struct stat *st = file_info(token_string(machine_pop_exc(m, token_STRING)));
  machine_push(m, token_new_int(0, S_ISLNK(st->st_mode)));
}

static void modified (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_int(0, file_modified(path)));
}

static void size (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_int(0, file_size(path)));
}

static void swrite (Machine *m) {
  char *text = token_string(machine_pop_exc(m, token_STRING));
  file_write(token_string(machine_pop_exc(m, token_STRING)), text);
}

static void append (Machine *m) {
  char *text = token_string(machine_pop_exc(m, token_STRING));
  file_append(token_string(machine_pop_exc(m, token_STRING)), text);
}

static void sread (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, file_read(path)));
}

static void aopen (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_from_pointer(symbol_FILE_, file_aopen(path)));
}

static void ropen (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_from_pointer(symbol_FILE_, file_ropen(path)));
}

static void wopen (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_from_pointer(symbol_FILE_, file_wopen(path)));
}

static void sclose (Machine *m) {
  file_close((FileLck *)fails_read_pointer(m, symbol_FILE_, machine_pop(m)));
}

static void readbin (Machine *m) {
  FileLck *f = fails_read_pointer(m, symbol_FILE_, machine_pop(m));
  machine_push(m, token_from_pointer(symbol_BYTES_, file_read_bin(f)));
}

static void readline (Machine *m) {
  FileLck *f = fails_read_pointer(m, symbol_FILE_, machine_pop(m));
  machine_push(m, token_new_string(0, file_read_line(f)));
}

static void writebin (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  FileLck *f = fails_read_pointer(m, symbol_FILE_, machine_pop(m));
  file_write_bin(f, bs);
}

static void writetext (Machine *m) {
  char *s = token_string(machine_pop_exc(m, token_STRING));
  FileLck *f = fails_read_pointer(m, symbol_FILE_, machine_pop(m));
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

  add("readBin", readbin); // STRING - BLOB
  add("readLine", readline); // STRING - STRING
  add("writeBin", writebin); // [STRING, BLOB] - []
  add("writeText", writetext); // [STRING, STRING] - []  (file, text) -[]

  return r;
}




