// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/rdCtx.h"

RdCtx *rdCtx_new (char *fpath, char *id) {
  RdCtx *this = MALLOC(RdCtx);
  this->fpath = fpath;
  this->id = id;
  this->imports = map_new();
  this->exports = arr_new();
  this->symMods = dicLayers_new();
  return this;
}
