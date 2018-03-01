/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <unistd.h>
#include <dm/dm.h>
#include "../include/initialize.h"
#include "../include/files.h"

void initialize(int is_server, char *app, char *dir, char *www, char *cgi) {
  void mkdir (char *d) {
    char *path = path_cat(dir, d, NULL);
    if (!file_exists(path)) {
      file_mkdir(path);
    }
  }

  void mkfile (char *d, char *tx) {
    char *path = path_cat(dir, d, NULL);
    if (!file_exists(path)) {
      file_write(path, tx);
    }
  }

  void mklink (char *root, char *old_name, char *new_name) {
    char *tmp = realpath(path_cat(dir, old_name, NULL), NULL);
    char *old_path = str_copy(tmp);
    free(tmp);
    char *new_path = path_cat(root, new_name, NULL);
    if (!file_exists(old_path)) {
      printf("'%s' not found", old_path);
      exit(1);
    }
    if (!file_exists(new_path)) {
      symlink(old_path, new_path);
    }
  }

  mkdir("www");
  mkdir("www/main");
  mkdir("www/img");
  mkfile("www/index.html", www_index());
  mkfile("www/styles.css", www_styles());
  mkfile("www/main/index.html", www_dir_index("../", app));
  mkfile("www/img/index.html", www_img_index());

  mkdir("i18n");
  mkfile("i18n/en.txt", "");
  mkfile("i18n/es.txt", "");
  mkdir("src");
  mkdir("src/lib");
  mkdir("src/main");
  mkfile("src/main/build.hxml", files_build("../../", "main", "Main"));
  mkfile("src/main/Main.hx", files_main("Main"));

  mklink(www, "www", app);

  if (is_server) {
    mkdir("scripts");
    mkdir("scripts/main");
    mkfile("src/main/buildS.hxml", files_buildS("../../", "main", "MainS"));
    mkfile("src/main/MainS.hx", files_mainS("MainS"));

    char *scripts = path_cat(cgi, "scripts", NULL);
    mklink(scripts, "scripts", app);
  }

}
