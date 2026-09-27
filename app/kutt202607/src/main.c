// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "main.h"
#include <signal.h>
#include "kut/sys.h"
#include "kut/str.h"
#include "kut/opt.h"
#include "kut/path.h"
#include "kut/file.h"
#include "data/indexEntry.h"
#include "comp/compiler.h"
#include "db.h"
#include "cts.h"
#include "fns.h"
#include "modules.h"
#include "doc.h"
#include "tests/all.h"

static void help () {
  puts(
    "Use:"
    "\n  kutt -h"
    "\n  kutt -v"
    "\n  kutt --clean"
    "\n  kutt --delete"
    "\n  kutt kttFile"
    "\n  kutt -c kttFile"
    "\n  kutt -o binFile kttFile"
    "\n  kutt -d kttFile"
    "\n"
    "\n  where"
    "\n    'kutt -h' show more command information."
    "\n    'kttFile' is a file with Kutt code."
    "\n      This file must end with '.ktt'. But you can write it"
    "\n      with or without the extension."
  );
  exit(0);
};

static void help2 () {
  puts(
    "Use:"
    "\n  kutt -h"
    "\n  kutt -v"
    "\n  kutt --clean"
    "\n  kutt --delete"
    "\n  kutt kttFile args..."
    "\n  kutt -c kttFile"
    "\n  kutt -o binFile kttFile"
    "\n"
    "\n  where"
    "\n    'kutt -h' show more information about the command."
    "\n    'kttFile' is a file with Kutt code."
    "\n      This file must end with '.ktt'. But you can write it"
    "\n      with or without the extension."
    "\n"
    "\n  kutt -h"
    "\n  Show this message."
    "\n"
    "\n  kutt -v"
    "\n  Show Kutt version."
    "\n"
    "\n  kutt --clean"
    "\n  Remove not necessary files in Kutt data base."
    "\n"
    "\n  kutt --delete"
    "\n  Remove all the files of Kutt data base."
    "\n"
    "\n  kutt kttFile args..."
    "\n  Compiles and runs 'kttFile'."
    "\n  Examples:"
    "\n  kutt myPrg.ktt    -> Compiles and execute myPrg.ktt"
    "\n  kutt myPrg        -> Compiles and execute myPrg.ktt"
    "\n  kutt myPrg a 12   -> Compiles and execute myPrg.ktt with 'a' and '12"
    "\n                       as parameters."
    "\n"
    "\n  kutt -c kttFile"
    "\n  Only compiles 'kttFile'."
    "\n  Examples:"
    "\n  kutt -c myPrg.ktt    -> Compiles myPrg.ktt"
    "\n  kutt -c myPrg        -> Compiles myPrg.ktt"
    "\n"
    "\n  kutt -o binFile kttFile"
    "\n  Compiles 'kttFile' and copy the binary as 'binFile'"
    "\n  Examples:"
    "\n  kutt -o /prgs/myPrg myPrg.ktt -> Compiles myPrg.ktt as /prgs/myPrg"
    "\n  kutt -o /prgs/myPrg myPrg     -> Compiles myPrg.ktt as /prgs/myPrg"
    "\n"
    "\n  kutt -d kttFile"
    "\n  Returns documentation of 'kttFile' in stdout."
    "\n  Examples:"
    "\n  kutt -d myPrg.ktt    -> Show documentation of myPrg.ktt"
    "\n  kutt -d myPrg        -> Show documentation of myPrg.ktt"
  );
  exit(0);
};


static void shandler (int sig) {
  EXC_KUTT("Arithmetic exception");
}

static void sshandler (int sig) {
  EXC_KUTT("Segmentation violation");
}

int main(int argc, char *argv[]) {
  GC_INIT();
  sys_init();
  modules_init();

  signal (SIGFPE, shandler);
  signal (SIGSEGV, sshandler);

  if (argc < 2) help();

  int only_compile = FALSE;
  int is_doc = FALSE;
  Opt *binary = opt_none(); // ([s])
  if (argv[1][0] == '-') {
    char *arg = argv[1];

    if (str_eq(arg, "--delete")) {
      db_delete();
      puts("All the files of data base deleted");
      return 0;
    } else if (str_eq(arg, "--clean")) {
      db_clean();
      puts("Data base cleaned");
      return 0;
    } else if (str_eq(arg, "--tests")) {
      all_run();
      return 0;
    } else if (str_eq(arg, "-h")) {
      help2();
    } else if (str_eq(arg, "-v")) {
      if (argc != 2) help();
      puts(str_f("Kutt version %s", cts_version));
      return 0;
    } else if (str_eq(arg, "-c")) {
      if (argc != 3) help();
      only_compile = TRUE;
    } else if (str_eq(arg, "-o")) {
      if (argc != 4) help();
      binary = opt_some(argv[2]);
    } else if (str_eq(arg, "-d")) {
      if (argc != 3) help();
      is_doc = TRUE;
    } else {
      printf("Unknown option %s\n", argv[1]);
      help();
    }
  }

  char *ktt_file = only_compile || is_doc
    ? argv[2]
    : opt_get(binary)
      ? argv[3]
      : argv[1]
  ;

  char *rpath = str_ends(ktt_file, ".ktt") ? str_left(ktt_file, -4) : ktt_file;

  if (is_doc) {
    char *f = str_f("%s.ktt", rpath);
    char *tx = file_exists(f) ? file_read(f) : "";
    printf("%s", doc_make(f, path_base(rpath), tx));
    return 0;
  }

  IndexEntry *e = db_init(rpath);
  char *path = e->path;
  char *id = e->id;
  if (!*path && !*id) {
    printf("File '%s.ktt' not found\n", rpath);
    return 0;
  }

  char *err = modules_add(TRUE, path, id);
  if (*err) {
    puts(fns_cut_msg(err, 500));
    return 0;
  }

  int skip = FALSE;
  char *bin = path_cat(cts_compilation_path(), str_f("%s.bin", id), NULL);

  if (file_exists(bin)) {
    skip = TRUE;
    int64_t exe_tm = file_modified(bin);
    void feach (Module *m) {
      if (file_modified(m->path) > exe_tm) skip = FALSE;
    }
    modules_each(feach);
  }

  if (skip) {
    if (!opt_get(binary) && !only_compile) {
      // Arr<char>
      Arr *cargs = arr_new();
      arr_push(cargs, bin);
      for (int i = 2; i < argc; ++i) arr_push(cargs, argv[i]);

      int rs = system(arr_cjoin(cargs, ' '));
      if (rs == -1) printf("Program can not be run");
      if (rs != 0) printf("Program exit with status %d", rs);
    }
    return 0;
  }

  err = compiler_run(only_compile);
  if (*err) {
    puts(err);
    return 0;
  }

  if (only_compile) return 0;

  char *bint = opt_get(binary);
  if (!bint) {
    // Arr<char>
    Arr *cargs = arr_new();
    arr_push(cargs, bin);
    for (int i = 2; i < argc; ++i) arr_push(cargs, argv[i]);

    int rs = system(arr_cjoin(cargs, ' '));
    if (rs == -1) printf("Program can not be run");
    if (rs != 0) printf("Program exit with status %d", rs);
    return 0;
  }

  file_copy(bin, bint);
  system(str_f("chmod +x %s", bint));
}

