// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers/reader.h"
#include "dmc/ext.h"
#include "dmc/Dec.h"
#include "io.h"

// keys is Varr[char]
static int start_ix(char *page, int ix, Varr *keys) {
  int r = -1;
  EACH(keys, char, k)
    r = str_index_from(page, k, ix);
    if (r == -1) break;
    r += strlen(k);
    ix = r;
  _EACH
  return r;
}

// keys is Varr[char]
static int end_ix(char *page, int ix, Varr *keys) {
  int r = -1;
  EACH(keys, char, k)
    r = str_index_from(page, k, ix);
    if (r == -1) break;
    ix = r;
  _EACH
  return r;
}

int reader_run(
  // Arr[char]
  Arr **codes_new,
  Darr **qs_new,
  char *path,
  int isIso,
  // Varr[char]
  Varr *tstart,
  // Varr[char]
  Varr *tend,
  // Varr[char]
  Varr *rstart,
  // Varr[char]
  Varr *rend,
  // Varr[char]
  Varr *c1start,
  // Varr[char]
  Varr *c1end,
  // Varr[char]
  Varr *c2start,
  // Varr[char]
  Varr *c2end
) {
  // Arr[char]
  Arr *codes = arr_new(free);
  Darr *qs = darr_new();
  *codes_new = codes;
  *qs_new = qs;

  int isEstrategias = str_starts(path, "https://www.estrategias");

  char *page = ext_wget_new(path);
  if (!*page) {
    io_loge("Fallo leyendo %s", path);
    return 1;
  }

  if (isIso) {
    char *tmp = str_from_iso_new(page);
    free(page);
    page = tmp;
  }

  int ts_ix = start_ix(page, 0, tstart);
  if (ts_ix == -1) {
    io_loge("Comienzo de tabla no encontrado en %s", path);
    return 1;
  }

  int te_ix = end_ix(page, ts_ix, tend);
  if (te_ix == -1) {
    io_loge("Final de tabla no encontrado en %s", path);
    return 1;
  }

  char *code = str_new("Inicio");
  int re_ix = ts_ix;
  for(;;) {
    int rs_ix = start_ix(page, re_ix, rstart);
    if (rs_ix == -1 || rs_ix > te_ix) break;

    re_ix = end_ix(page, rs_ix, rend);
    if (re_ix == -1) {
      io_loge("Final de fila no encontrado despues de '%s'", code);
      re_ix = rs_ix;
      continue;
    }

    int c1s_ix = start_ix(page, rs_ix, c1start);
    if (c1s_ix == -1 || c1s_ix > re_ix) {
      io_logw("Comienzo del campo 'code' no encontrado despues de '%s'", code);
      continue;
    }
    int c1e_ix = end_ix(page, c1s_ix, c1end);
    if (c1s_ix == -1 || c1s_ix > re_ix) {
      io_loge("Final del campo 'code' no encontrado despues de '%s'", code);
      continue;
    }
    free(code);
    Buf *bf = buf_new();
    buf_add_buf(bf, page + c1s_ix, c1e_ix - c1s_ix);
    code = str_trim_new(buf_str(bf));
    free(bf);

    int c2s_ix = start_ix(page, c1e_ix, c2start);
    if (c2s_ix == -1 || c2s_ix > re_ix) {
      if (
        isEstrategias &&
        ( str_eq(code, "Cleop") ||
          str_eq(code, "CAM") ||
          str_eq(code, "Adveo"))
      ) {
        continue;
      }
      io_logw("Comienzo del campo 'quote' no encontrado en '%s'", code);
      continue;
    }
    int c2e_ix = end_ix(page, c2s_ix, c2end);
    if (c2s_ix == -1 || c2s_ix > re_ix) {
      io_loge("Final del campo 'quote' no encontrado en '%s'", code);
      continue;
    }
    bf = buf_new();
    buf_add_buf(bf, page + c2s_ix, c2e_ix - c2s_ix);
    char *q = str_trim_new(buf_str(bf));
    free(bf);
    dec_regularize_iso(&q);
    if (!dec_number(q)) {
      io_logw("Número no válido '%s' en '%s'", q, code);
      continue;
    }

    arr_push(codes, str_new(code));
    darr_push(qs, atof(q));

    free(q);
  }
  free(code);

  return 0;
}
