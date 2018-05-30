// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "dmc/sys.h"
#include "dmc/Achar.h"
#include "dmc/exc.h"
#include "dmc/rnd.h"
#include "dmc/path.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/DEFS.h"

static struct {
  char *home;
  char *uname;
} sys;

void sys_init (char *path) {
  XNULL(path)

  exc_init();
  rnd_init();

  uid_t uid = getuid();
  struct passwd *udata = getpwuid(uid);
  sys.home = path_cat(udata->pw_dir, ".dmCApp", path, NULL);
  sys.uname = str_copy(udata->pw_name);
  file_mkdir(sys.home);
}

inline
char *sys_home (void) {
  return sys.home;
}

inline
char *sys_uname (void) {
  return sys.uname;
}

void sys_locale (char *language) {
  XNULL(language)
  setlocale (LC_ALL, language);
}

Achar *sys_cmd(char *command) {
  XNULL(command)

  Achar *r = achar_new();
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  fp = popen(str_printf("%s 2>&1", command), "r");
  if (!fp)
    return NULL;

  while (getline(&line, &len, fp) != -1) {
    achar_add(r, str_copy(line));
    free(line);
    line = NULL;
  }
  free(line);
  return r;
}

