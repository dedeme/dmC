// Copyright 11-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers.h"
#include "DEFS.h"
#include "io.h"
#include "servers/estrategias.h"
#include "servers/expansion.h"
#include "servers/finanzas.h"
#include "servers/infobolsa.h"
#include "servers/invertia.h"
#include "servers/libremercado.h"

static Js *read_data_new(void) {
  char *path = path_cat_new(QUOTES, "daily.db", NULL);
  Js *r = (Js *)file_read_new(path);
  free(path);
  return r;
}

Arr *servers_all_names_new(void) {
  Js *d = read_data_new();
  // Map[Js]
  Map *m = js_ro_new(d);
  Js *svs_js = map_get_null(m, "servers");
  if (!svs_js) FAIL("Null pointer")

  // Map[Js]
  Map *svs = js_ro_new(svs_js);
  // Arr[char]
  Arr *names = arr_new(free);
  EACH(map_kvs(svs), Kv, kv)
    arr_push(names, str_new(map_key(kv)));
  _EACH

  map_free(svs);
  map_free(m);

  return names;
}

Arr *servers_all_selected_names_new(void) {
  Js *d = read_data_new();
  // Map[Js]
  Map *m = js_ro_new(d);
  Js *svs_js = map_get_null(m, "servers");
  if (!svs_js) FAIL("Null pointer")

  // Map[Js]
  Map *svs = js_ro_new(svs_js);
  // Arr[char]
  Arr *names = arr_new(free);
  EACH(map_kvs(svs), Kv, kv)
    // Arr[Js]
    Arr *vjs = js_ra_new(map_value(kv));
    int sel = js_rb(arr_get(vjs, 0));
    arr_free(vjs);
    if (sel) {
      arr_push(names, str_new(map_key(kv)));
    }
  _EACH

  map_free(svs);
  map_free(m);

  return names;
}

void servers_data(char **url_new, Map **codes_new, char *server) {
  Js *d = read_data_new();

  // Map[Js]
  Map *m = js_ro_new(d);
  Js *svs_js = map_get_null(m, "servers");
  if (!svs_js) FAIL("Null pointer")
  Js *cos_js = map_get_null(m, "cos");
  if (!cos_js) FAIL("Null pointer")

  // Map[Js]
  Map *svs = js_ro_new(svs_js);
  int ix = -1;
  int c = 0;
  EACH(map_kvs(svs), Kv, kv)
    char *name = map_key(kv);
    if (str_eq(name, server)) {
      ix = c;
      Js *v = map_value(kv);
      // Arr[js]
      Arr *selUrl = js_ra_new(v);
      *url_new = js_rs_new(arr_get(selUrl, 1));
      arr_free(selUrl);
      break;
    }
    ++c;
  _EACH

  if (ix == -1) FAIL (str_f_new("Server %s not found", server))

  // Map[Js]
  Map *cos = js_ro_new(cos_js);
  // Map[char]
  Map *codes = map_new(free);
  EACH(cos, Kv, kv)
    Js *v = map_value(kv);
    // Arr[Js]
    Arr *selCodes = js_ra_new(v);

    if (js_rb(arr_get(selCodes, 0))) {
      char *k = map_key(kv);
      // Arr[Js]
      Arr *cs = js_ra_new(arr_get(selCodes, 1));
      map_put(codes, k, js_rs_new(arr_get(cs, ix)));
      arr_free(cs);
    }

    arr_free(selCodes);
  _EACH

  *codes_new = codes;

  map_free(cos);
  map_free(svs);
  map_free(m);
}

int servers_read_quotes(Varr **nicks_new, Darr **qs_new, char *id) {
  if (str_eq(id, estrategias_name()))
    return estrategias_read(nicks_new, qs_new);
  else if (str_eq(id, expansion_name()))
    return expansion_read(nicks_new, qs_new);
  else if (str_eq(id, finanzas_name()))
    return finanzas_read(nicks_new, qs_new);
  else if (str_eq(id, infobolsa_name()))
    return infobolsa_read(nicks_new, qs_new);
  else if (str_eq(id, invertia_name()))
    return invertia_read(nicks_new, qs_new);
  else if (str_eq(id, libremercado_name()))
    return libremercado_read(nicks_new, qs_new);
  else
    FAIL(str_f_new("Server '%s' not found", id));
  return 1;
}

void servers_tests (void) {
  puts("\nSelected servers:");
  puts(  "----------------");
  // Arr[char]
  Arr *svs = servers_all_selected_names_new();
  EACH(svs, char, sv)
    puts(sv);
  _EACH
  arr_free(svs);

  puts("\nAll servers:");
  puts(  "-----------");
  svs = servers_all_names_new();
  EACH(svs, char, sv)
    puts(sv);
  _EACH

  puts("\nNicks:");
  puts(  "-----");
  // Arr[char]
  Arr *nicks = io_nicks_new();
  EACH(nicks, char, nick)
    puts(nick);
  _EACH

  puts("\nReading quotes:");
  puts(  "--------------");

  EACH(svs, char, sv)
    Varr *nicks_new;
    Darr *qs_new;
    if (servers_read_quotes(&nicks_new, &qs_new, sv))
      FAIL (str_f_new("Fail reading quotes of %s", sv))
    int ix = 0;
    EACH(nicks_new, char, nk)
      int fpred(char *e) {
        return str_eq(nk, e);
      }
      if (arr_index(nicks, (FPRED)fpred) == -1) {
        char *msg = str_f_new(
          "%s is in %s, but it is not a selected nick", nk, sv
        );
        puts(msg);
        free(msg);
      }

      double q = darr_get(qs_new, ix++);
      if (q <= 0) {
        char *msg = str_f_new("Fail reading quote of %s in %s", nk, sv);
        puts(msg);
        free(msg);
      }
    _EACH

    EACH(nicks, char, nick)
      int fpred(char *e) {
        return str_eq(nick, e);
      }
      if (varr_index(nicks_new, (FPRED)fpred) == -1) {
        char *msg = str_f_new(
          "%s is not in %s, but it is a selected nick", nick, sv
        );
        puts(msg);
        free(msg);
      }

    _EACH

    varr_free(nicks_new);
    darr_free(qs_new);
    char *msg = str_f_new("%s correct", sv);
    puts(msg);
    free(msg);
  _EACH

  arr_free(svs);
  arr_free(nicks);
}
