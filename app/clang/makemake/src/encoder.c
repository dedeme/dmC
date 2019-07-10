// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "encoder.h"
#include "DEFS.h"

char *encoder_mk (Arr *ns) {
  char *lib_dir = str_f("%s/%s/", MK_DIR, LIB_DIR);

  Buf *bf = buf_new();
  buf_add(bf, "#!/bin/bash\n\nROOT=$(pwd)\n\n");
  EACHR(ns, Node2, n)
    if (node2_depth(n)) {
      buf_add(bf, "cd $ROOT/");
      buf_add(bf, lib_dir);
      buf_cadd(bf, '/');
      buf_add(bf, node2_path(n));
      buf_add(bf, "\nmake > /dev/null\n");
    }
  _EACH
  buf_add(bf, "cd $ROOT\nmake > /dev/null\n");
  return buf_to_str(bf);
}

char *encoder_makefile (Arr *libs, Node2 *n) {
  int terminal_node = !arr_size(node2_children(n));

  Buf *bf = buf_new();

  char *dir = node2_path(n);
  char *name = path_name(dir);

  buf_add(bf, "ROOT = ../..");
  REPEAT(arr_size(str_csplit(dir, '/')))
    buf_add(bf, "/..");
  _REPEAT
  buf_cadd(bf, '\n');

  buf_add(bf, "NAME = ");
  buf_add(bf, name);
  buf_cadd(bf, '\n');

  buf_add(bf, "DIR = ");
  buf_add(bf, dir);
  buf_cadd(bf, '\n');
  buf_cadd(bf, '\n');

  EACH(libs, char, lib)
    buf_add(bf, "LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, " = $(ROOT)/lib/lib");
    buf_add(bf, lib);
    buf_cadd(bf, '\n');
    buf_add(bf, "LIB_INCLUDE_");
    buf_add(bf, lib);
    buf_add(bf, " = $(LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, ")/include\n");
    buf_cadd(bf, '\n');
  _EACH

  char *obj = str_f("%s/%s", MK_DIR, OBJ_DIR);
  buf_add(bf, "OS = $(subst src,");
  buf_add(bf, obj);
  buf_add(bf, ",$(patsubst %.c,%.o,$(wildcard $(ROOT)/src/$(DIR)/*.c)))\n\n");

  buf_add(bf, "lib$(NAME).a : $(OS)\n");
  buf_add(bf, "\tar rcs lib$(NAME).a $(OS)\n\n");

  buf_add(bf, "CFLAGS = -Wall -rdynamic\n");
  buf_add(bf, "FILES = $(wildcard $(ROOT)/src/$(DIR)/*.c) ");
  buf_add(bf, "$(wildcard $(ROOT)/include/$(DIR)/*.h)\n");
  buf_add(bf, "$(ROOT)/");
  buf_add(bf, obj);
  buf_add(bf, "/$(DIR)/%.o : ");
  buf_add(bf, "$(ROOT)/src/$(DIR)/%.c $(ROOT)/include/$(DIR)/%.h \\\n");
  buf_add(bf, "\t\t$(FILES)");

  if (terminal_node) {
    EACH(libs, char, lib)
      buf_add(bf, " \\\n\t\t$(LIB_NAME_");
      buf_add(bf, lib);
      buf_add(bf, ")");
    _EACH
  } else {
    char *parent = str_f("$(ROOT)/%s/%s/", MK_DIR, LIB_DIR);
    EACH(node2_children(n), Node2, ch)
      char *chdir = node2_path(ch);
      char *chname = path_name(chdir);
      buf_add(bf, " \\\n\t\t");
      buf_add(bf, parent);
      buf_add(bf, chdir);
      buf_add(bf, "/lib");
      buf_add(bf, chname);
      buf_add(bf, ".a");
    _EACH
  }
  buf_cadd(bf, '\n');
  buf_add(bf, "\tgcc $(CFLAGS) -c $< -o $@ \\\n");
  buf_add(bf, "\t\t-I$(ROOT)/include");
  EACH(libs, char, lib)
    buf_add(bf, " \\\n\t\t-I$(LIB_INCLUDE_");
    buf_add(bf, lib);
    buf_add(bf, ")");
  _EACH
  buf_cadd(bf, '\n');

  return buf_to_str(bf);
}

char *encoder_main_makefile (char *prg, Arr *libs, Arr *ns, Node2 *n) {
  char *lib_dir = str_f("%s/%s/", MK_DIR, LIB_DIR);

  int terminal_node = !arr_size(node2_children(n));

  Buf *bf = buf_new();

  buf_add(bf, "PRG = ");
  buf_add(bf, prg);
  buf_cadd(bf, '\n');
  buf_cadd(bf, '\n');

  EACH(libs, char, lib)
    buf_add(bf, "LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, " = lib/lib");
    buf_add(bf, lib);
    buf_cadd(bf, '\n');
    buf_add(bf, "LIB_INCLUDE_");
    buf_add(bf, lib);
    buf_add(bf, " = $(LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, ")/include\n");
    buf_add(bf, "LIB_NAME_");
    buf_add(bf, lib);
    buf_add(bf, " = $(LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, ")/lib");
    buf_add(bf, lib);
    buf_add(bf, ".a\n");
    buf_cadd(bf, '\n');
  _EACH

  buf_add(bf, "CFLAGS = -Wall -rdynamic\n");
  buf_add(bf, "FILES = $(wildcard src/*.c) $(wildcard include/*.h)\n");
  buf_add(bf, "bin/");
  buf_add(bf, prg);
  buf_add(bf, ": $(FILES)");

  if (terminal_node) {
    EACH(libs, char, lib)
      buf_add(bf, " \\\n\t\t$(LIB_NAME_");
      buf_add(bf, lib);
      buf_add(bf, ")");
    _EACH
  } else {
    EACH(node2_children(n), Node2, ch)
      char *chdir = node2_path(ch);
      char *chname = path_name(chdir);
      buf_add(bf, " \\\n\t\t");
      buf_add(bf, lib_dir);
      buf_add(bf, chdir);
      buf_add(bf, "/lib");
      buf_add(bf, chname);
      buf_add(bf, ".a");
    _EACH
  }
  buf_cadd(bf, '\n');
  buf_add(bf, "\tgcc $(CFLAGS) $(FILES) -o bin/");
  buf_add(bf, prg);
  buf_add(bf, " \\\n");
  buf_add(bf, "\t\t-Iinclude");
  EACH(libs, char, lib)
    buf_add(bf, " \\\n\t\t-I$(LIB_INCLUDE_");
    buf_add(bf, lib);
    buf_add(bf, ")");
  _EACH
  EACH(ns, Node2, sn)
    if (node2_depth(sn)) {
      char *sdir = node2_path(sn);
      char *sname = path_name(sdir);
      buf_add(bf, " \\\n\t\t-L");
      buf_add(bf, lib_dir);
      buf_add(bf, sdir);
      buf_add(bf, " -l");
      buf_add(bf, sname);
    }
  _EACH
  EACH(libs, char, lib)
    buf_add(bf, " \\\n\t\t-L$(LIB_PATH_");
    buf_add(bf, lib);
    buf_add(bf, ") -l");
    buf_add(bf, lib);
  _EACH
  buf_add(bf, " \\\n\t\t-lgc -lpthread -lm");
  buf_cadd(bf, '\n');

  return buf_to_str(bf);
}
