// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db/quotesDb.h"
#include "kut/DEFS.h"
#include "kut/path.h"
#include "kut/file.h"
#include "mkt/qs.h"
#include "data/cts.h"
#include "db.h"


static char *dpath = "quotes";

static char *fpath (char *nick) {
  return path_cat(dpath, str_f("%s.tb", nick), NULL);
}

void quotesDb_init (void) {
  if (!db_exists(dpath)) file_mkdir(db_path(dpath));
}

// <char>
Arr *quotesDb_nicks (void) {
    //--
    int filter (char *f) { return str_ends(f, ".tb"); }
    char *map (char *f) { return str_left(f, -3); }
  return arr_map(
    arr_filter_to(
      file_dir(db_path(dpath)),
      (FPRED)filter
    ),
    (FMAP)map
  );
}

char *quotesDb_read (char *nick) {
  return db_read(fpath(nick));
}

Rs *quotesDb_read_qs (char *nick) {
  return qs_from_str(HISTORIC_QUOTES, quotesDb_read(nick));
}

void quotesDb_write (char *nick, char *data) {
  db_write(fpath(nick), data);
}
