// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader context.

#ifndef DATA_RD_CTX_H
  #define DATA_RD_CTX_H

#include "kut/map.h"
#include "data/dicLayers.h"

/// Structure.
typedef struct {
  char *fpath;        // Canonical path of module file.
  char *id;           // C identifier of module (id.h and id.c will be created)
  Map *imports;       // (Map<Imp>) Dictionary of imports. {sym::moduleId}
  Arr *exports;       // (Arr<TypedSym>) List of symbols to export.
  DicLayers *symMods; // DicLayers of Map<char>::{symbol: module} that associates
                      // expression symbols with local names of imported modules.
} RdCtx;

/// Constructor
///   fpath  : Canonical path of module file.
///   id     : C identifier of module (id.h and id.c will be created)
RdCtx *rdCtx_new (char *fpath, char *id);

#endif
