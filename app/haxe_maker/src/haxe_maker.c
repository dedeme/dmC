/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <dmc/all.h>
#include <unistd.h>
#include "../include/help.h"
#include "../include/initialize.h"
#include "../include/compile.h"
#include "../include/new_directory.h"
#include "../include/dic.h"

char *path_regularize(char *path, int absolute) {
  while (str_index(path, "/.") != -1) path = str_replace(path, "/.", "/");
  while (str_index(path, "//") != -1) path = str_replace(path, "//", "/");
  if (!*path) return "";
  if (*path == '/' && !absolute) return "";
  if (*path != '/' && absolute) return "";
  while (path[strlen(path) - 1] == '/')
    path = str_sub(path, 0, strlen(path) - 1);
  return path;
}

int main (int argc, char **argv) {
  char *dir = path_parent(argv[0]);

  int is_server = 0;
  int is_init = 0;
  int is_dir = 0;
  int is_comp = 0;
  int is_i18n = 0;
  char *app = NULL;
  char *www = NULL;
  char *cgi = NULL;
  char *new_dir = NULL;

  int i;
  for (i = 1; i < argc; ++i) {
    char *arg = argv[i];
    if (!strcmp(arg, "-s")) is_server = 1;
    else if (!strcmp(arg, "-init")) is_init = 1;
    else if (!strcmp(arg, "-i18n")) is_i18n = 1;
    else if (str_starts(arg, "-dir=")) {
      is_dir = 1;
      new_dir = path_regularize(str_sub(arg, 5, strlen(arg)), 0);
    }
    else if (!strcmp(arg, "-comp")) is_comp = 1;
    else if (str_starts(arg, "-app=")) app = str_sub(arg, 5, strlen(arg));
    else if (str_starts(arg, "-www="))
      www = path_regularize(str_sub(arg, 5, strlen(arg)), 1);
    else if (str_starts(arg, "-cgi="))
      cgi = path_regularize(str_sub(arg, 5, strlen(arg)), 1);
    else {
      printf("'%s': Unknown option\n", arg);
      help();
      return 0;
    }
  }

  if (is_dir + is_init + is_comp + is_i18n > 1) {
    puts("Options '-init', '-i18n', '-comp' and '-dir' are incompatibles");
    return 0;
  }

  if (app) {
    if (!*app) {
      puts("app= : Application name is missing\n");
      return 0;
    }
  }

  if (new_dir) {
    if (!*new_dir) {
      puts("dir= : Directory is missing or starts with '/'\n");
      return 0;
    }
  }

  if (www) {
    if (!*www) {
      puts("www= : Directory is missing or does not start with '/'\n");
      return 0;
    }
    if (!file_is_directory(www)) {
      printf("www='%s': Directory not found\n", www);
      return 0;
    }
  }

  if (cgi) {
    if (!*cgi) {
      puts("cgi= : Directory is missing or does not start with '/'\n");
      return 0;
    }
    if (!file_is_directory(cgi)) {
      printf("cgi='%s': Directory not found\n", cgi);
      return 0;
    }
  }

  if (is_init) {
    if (!app) {
      puts("Option -app is missing");
      help();
      return 0;
    }
    if (!www) {
      puts("Option -init without -www.");
      help();
      return 0;
    }
    if (!is_server && argc != 4) {
      puts("Bad number of arguments.");
      help();
      return 0;
    }
    if (is_server && !cgi) {
      if (argc != 6) {
        puts("Bad number of arguments.");
        help();
        return 0;
      }
      puts("Option -init -s without -cgi.");
      help();
      return 0;
    }
    initialize(is_server, app, dir, www, cgi);
    return 0;
  }

  if (is_dir) {
    if (is_server && argc == 3) {
      new_directory(new_dir, 1);
    } else if (argc == 2) {
      new_directory(new_dir, 0);
    } else {
      puts("Bad number of arguments.");
      help();
    }
    return 0;
  }

  if (is_comp) {
    if (!is_server && argc != 2) {
      puts("Bad number of arguments.");
      help();
      return 0;
    }

    if (is_server) {
      if (argc != 3) {
        puts("Bad number of arguments.");
        help();
        return 0;
      }
    }
    compile(is_server);
    return 0;
  }

  if (is_i18n) {
    if (!is_server && argc != 2) {
      puts("Bad number of arguments.");
      help();
      return 0;
    }

    dic_make();
    return 0;
  }

  help();

  return 0;
}

