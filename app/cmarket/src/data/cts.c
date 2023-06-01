// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/cts.h"

char *cts_app_name (void) {
  return "cmarket";
}

char *cts_version (void) {
  return "202306";
}

/// KtWeb data path
char *cts_data_path (void) {
  return "/dm/dmWeb/app/KtWeb/dmcgi/CMarket";
}
