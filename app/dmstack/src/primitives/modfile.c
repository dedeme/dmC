// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfile.h"
#include <unistd.h>
#include <sys/stat.h>
#include "Machine.h"
#include "primitives/modlong.h"

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
  machine_push(m, modlong_from_long(file_modified(path)));
}

static void size (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, modlong_from_long(file_size(path)));
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
  machine_push(m, modlong_from_pointer("= __File", file_aopen(path)));
}

static void ropen (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, modlong_from_pointer("= __File", file_ropen(path)));
}

static void wopen (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, modlong_from_pointer("= __File", file_wopen(path)));
}

static void sclose (Machine *m) {
  file_close((FileLck *)modlong_to_pointer(m, "= __File", machine_pop(m)));
}

static void readbin (Machine *m) {
  FileLck *f = modlong_to_pointer(m, "= __File", machine_pop(m));
  machine_push(m, token_new_blob(0, file_read_bin(f)));
}

static void readline (Machine *m) {
  FileLck *f = modlong_to_pointer(m, "= __File", machine_pop(m));
  machine_push(m, token_new_string(0, file_read_line(f)));
}

static void writebin (Machine *m) {
  Bytes *bs = token_blob(machine_pop_exc(m, token_BLOB));
  FileLck *f = modlong_to_pointer(m, "= __File", machine_pop(m));
  file_write_bin(f, bs);
}

static void writetext (Machine *m) {
  char *s = token_string(machine_pop_exc(m, token_STRING));
  FileLck *f = modlong_to_pointer(m, "= __File", machine_pop(m));
  file_write_text(f, s);
}

// Resturns Map<primitives_Fn>
Map *modfile_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "cwd", cwd); // [] - STRING
  map_put(r, "cd", cd); // STRING - []
  map_put(r, "mkdir", smkdir); // STRING - []
  map_put(r, "dir", dir); // STRING - LIST<STRING>
  map_put(r, "directory?", isdirectory); // STRING - INT

  map_put(r, "exists?", exists);  // STRING - INT
  map_put(r, "del", del); // STRING - []
  map_put(r, "rename", srename); // [STRING, STRING] - []   (old, new) - []
  map_put(r, "link", slink); // [STRING, STRING] - []   (old, new) - []
  map_put(r, "copy", copy); // [STRING, STRING] - []   (source, target) - []

  map_put(r, "regular?", isregular); // STRING - INT
  map_put(r, "link?", islink); // STRING - INT
  map_put(r, "modified", modified); // STRING - LONG
  map_put(r, "size", size); // STRING - LONG

  map_put(r, "write", swrite); // [STRING, STRING] - []   (path, text) - []
  map_put(r, "append", append); // [STRING, STRING] - []   (path, text) - []
  map_put(r, "read", sread); // STRING - STRING

  map_put(r, "aopen", aopen); // STRING - CPOINTER
  map_put(r, "ropen", ropen); // STRING - CPOINTER
  map_put(r, "wopen", wopen); // STRING - CPOINTER
  map_put(r, "close", sclose); // CPOINTER - []

  map_put(r, "readBin", readbin); // STRING - BLOB
  map_put(r, "readLine", readline); // STRING - STRING
  map_put(r, "writeBin", writebin); // [STRING, BLOB] - []
  map_put(r, "writeText", writetext); // [STRING, STRING] - []  (file, text) -[]

  return r;
}




