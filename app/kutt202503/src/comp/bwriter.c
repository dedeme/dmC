// Copyright 22-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/bwriter.h"
#include "data/wrSRs.h"
#include "comp/blockWriter.h"
#include "cts.h"

Tp *bwriter_write (Module *md) {
  WrSRs *rs = blockWriter_run(
    arr_new(), map_new(), blockWriter_top, FALSE, FALSE, wrCtx_new(md),
    0, md->code
  );
  return rs->is_error
    ? tp_mk(rs->code, "")
    : tp_mk("", rs->code)
  ;
}
