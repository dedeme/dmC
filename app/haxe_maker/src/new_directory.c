/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <dmc/all.h>
#include "../include/new_directory.h"
#include "../include/files.h"

char *path_level(int level) {
  char *r = "../";
  int i;
  for (i = 0; i < level; ++i) r = str_cat(r, "../", NULL);
  return r;
}

void new_directory (char* path, int is_server) {
  int level = 0;
  char *p = path;
  while (*p) if (*p++ == '/') ++level;

  char *name = path_name(path);
  name = str_printf("%s%s",
    str_to_upper(str_sub(name, 0, 1)), str_sub(name, 1, strlen(name)));

  char *dir = path_cat("src", path, NULL);
  if (file_exists(dir)) {
    printf("Warning: Directory '%s' already exists\n", dir);
  } else {
    file_mkdir(dir);
  }

  char *f = path_cat(dir, str_cat(name, ".hx", NULL), NULL);
  if (file_exists(f))
    printf("Warning: File '%s' already exists\n", f);
  else
    file_write(f, files_main(name));

  f = path_cat(dir, "build.hxml", NULL);
  if (file_exists(f))
    printf("Warning: File '%s' already exists\n", f);
  else
    file_write(f, files_build(path_level(level + 1), path, name));

  dir = path_cat("www", path, NULL);
  if (file_exists(dir)) {
    printf("Warning: Directory '%s' already exists\n", dir);
  } else {
    file_mkdir(dir);
  }

  f = path_cat(dir, "index.html", NULL);
  if (file_exists(f))
    printf("Warning: File '%s' already exists\n", f);
  else
    file_write(f, www_dir_index(path_level(level), path));

  if (is_server) {
    char *nameS = str_printf("%sS", name);
    dir = path_cat("src", path, NULL);

    char *f = path_cat(dir, str_cat(nameS, ".hx", NULL), NULL);
    if (file_exists(f))
      printf("Warning: File '%s' already exists\n", f);
    else
      file_write(f, files_mainS(nameS));

    f = path_cat(dir, "buildS.hxml", NULL);
    if (file_exists(f))
      printf("Warning: File '%s' already exists\n", f);
    else
      file_write(f, files_buildS(path_level(level + 1), path, nameS));

    dir = path_cat("scripts", path, NULL);
    if (file_exists(dir))
      printf("Warning: Directory '%s' already exists\n", dir);
    else {
      file_mkdir(dir);
    }
  }
}

