// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>
#include "market/Quote.h"
#include "io.h"
#include "Options.h"
#include "Quotes.h"
#include "DEFS.h"
#include "Flea.h"

static char *home = NULL;
static char *data_dir_base = NULL;
static char *data_dir_opt = NULL;
static char *tmp_dir = NULL;
static char *bolsa_data_dir = NULL;
static char *fversion = NULL;

void io_init(void) {
  home = sys_home();

  data_dir_base = path_cat(home, "data", NULL);
  if (!file_exists(data_dir_base)) {
    file_mkdir(data_dir_base);
    file_mkdir(path_cat(data_dir_base, "all", NULL));
    file_mkdir(path_cat(data_dir_base, "best", NULL));
    file_mkdir(path_cat(data_dir_base, "ibex", NULL));
    file_mkdir(path_cat(data_dir_base, "ibexBest", NULL));
  }

  fversion = path_cat(data_dir_base, "version.txt", NULL);
  if (!file_exists(fversion)) {
    file_write(fversion, DATA_VERSION);
  } else {
    char *v = file_read(fversion);
    if (strcmp(v, DATA_VERSION)) {
      THROW exc_illegal_argument("v", DATA_VERSION, v) _THROW
    }
  }

  Options *opts = options_get();
  data_dir_opt = options_ibex(opts)
    ? options_best(opts)
      ? path_cat(data_dir_base, "ibexBest", NULL)
      : path_cat(data_dir_base, "ibex", NULL)
    : options_best(opts)
      ? path_cat(data_dir_base, "best", NULL)
      : path_cat(data_dir_base, "all", NULL)
    ;

  tmp_dir = path_cat(home, "tmp", NULL);
  if (file_exists(tmp_dir)) {
    file_del(tmp_dir);
  }
  file_mkdir(tmp_dir);

  bolsa_data_dir = path_cat(home, "bolsa_data", NULL);
  if (!file_exists(bolsa_data_dir)) {
    THROW "Symbolic link to 'bolsa_data' is missing" _THROW
  }
}

Nicks *io_nicks(void) {
  char *data_db = path_cat(bolsa_data_dir, "data.db", NULL);
  Map/*Json*/ *data = json_robject(file_read(data_db));

  Map/*Json*/ *status = json_robject(map_get(data, "status"));

  if (arr_size((Arr *)status) != NICKS_NUMBER)
    THROW
      "arr_size(status) is %d, but ought to be %d",
      arr_size((Arr *)status), NICKS_NUMBER
    _THROW

  Nicks *nks = nicks_new(NICKS_NUMBER);
  RANGE0(i, NICKS_NUMBER) {
    Kv *kv = (Kv *)arr_get((Arr *)status, i);
    char *v = json_rstring(kv->value);
    if (*v) {
      THROW exc_illegal_argument(kv->key, "blank", v) _THROW
    }
    Nick *nick = nick_new(kv->key);
    nicks_set(nks, i, nick);
  }_RANGE

  Map/*Json*/ *ibex = json_robject(map_get(data, "ibex"));
  EACH((Arr *)ibex, Kv, kv) {
    bool v = json_rbool(kv->value);
    if (v) {
      RANGE0(i, NICKS_NUMBER){
        if (!strcmp(nick_id(nicks_get(nks, i)), kv->key)) {
          nick_set_in_ibex(nicks_get(nks, i));
          break;
        }
      }_RANGE
    }
  }_EACH

  return nks;
}

static Quote *quote_from_str(char *quote) {
  Arr/*char*/ *qparts = str_csplit(quote, ':');

  return quote_new(
    atof(arr_get(qparts, 1)),
    atof(arr_get(qparts, 2)),
    atof(arr_get(qparts, 3)),
    atof(arr_get(qparts, 4)),
    atoi(arr_get(qparts, 5))
  );
}

Arr/*char*/ *io_quotes(Nicks *nicks) {
  Arr/*char*/ *qdates = arr_new();
  Quote **quotes = quotes_get();

  char *path = path_cat(
    bolsa_data_dir,
    str_printf("%s.db", nick_id(nicks_get(nicks, 0))),
    NULL
  );
  Arr/*char*/ *lines = str_csplit(file_read(path), '\n');
  arr_reverse(lines);

  EACH(lines, char, line) {
    int ix = str_cindex(line, ':');
    arr_add(qdates, str_sub(line, 0, ix));
  }_EACH

  if (arr_size(qdates) != QUOTES_NUMBER)
    THROW
      "arr_size(qdates) is %d, but ought to be %d",
      arr_size(qdates), QUOTES_NUMBER
    _THROW

  RANGE0(nix, NICKS_NUMBER) {
    char *nick = nick_id(nicks_get(nicks, nix));
    char *path = path_cat(bolsa_data_dir, str_printf("%s.db", nick), NULL);
    Arr/*char*/ *lines = str_csplit(file_read(path), '\n');
    arr_reverse(lines);

    RANGE0(i, QUOTES_NUMBER) {
      char *line = arr_get(lines, i);
      int ix = str_cindex(line, ':');
      char *date = str_sub(line, 0, ix);
      if (strcmp(date, arr_get(qdates, i))) {
        THROW
          "io_quotes: Date does not match at index %d between %s and %s "
          "(%s : %s)",
          i, nick, nick_id(nicks_get(nicks, 0)),
          date, (char *)arr_get(qdates, i)
        _THROW
      }

      quotes[i * NICKS_NUMBER + nix] = quote_from_str(line);
    }_RANGE
  }_RANGE
  return qdates;
}

static Fleas *read_fleas() {
  Fleas *fleas = fleas_new(FLEAS_NUMBER);
  RANGE0(i, FLEAS_NUMBER / 1000) {
    char *path = path_cat(data_dir_opt, str_printf("fleas%d.db", i), NULL);
    Arr/*Json*/ *jsa = json_rarray(file_read(path));
    size_t ifl = i * 1000;
    EACH(jsa, Json, a) {
      fleas_set(fleas, ifl++, flea_restore(a));
    }_EACH
  }_RANGE
  return fleas;
}

void io_get_fleas(size_t *fleaId, size_t *cycle, Fleas **fleas) {
  char *path = path_cat(data_dir_opt, "conf.db", NULL);
  if (!file_exists(path)) {
    *fleas = fleas_new(FLEAS_NUMBER);
    RANGE0(i, FLEAS_NUMBER) {
      Flea *f = flea_new(i + 1, 0);
      fleas_set(*fleas, i, f);
    }_RANGE

    *fleaId = FLEAS_NUMBER + 1;
    *cycle = 0;

    io_set_fleas(FLEAS_NUMBER + 1, 0, *fleas);
    return;
  }

  Map/*Json*/ *jsm = json_robject(file_read(path));
  *fleaId = (size_t)jmap_guint(jsm, "fleaId");
  *cycle = (size_t)jmap_guint(jsm, "cycle");

  *fleas = read_fleas();
}

void io_set_fleas(size_t fleaId, size_t cycle, Fleas *fleas) {
  char *path = path_cat(data_dir_opt, "conf.db", NULL);

  Map/*Json*/ *jsm = map_new();
  jmap_puint(jsm, "fleaId", fleaId);
  jmap_puint(jsm, "cycle", cycle);
  file_write(path, json_wobject(jsm));

  size_t i = 0;
  size_t size = fleas_size(fleas);
  while (i < size) {
    size_t end = i + 1000;
    if (end > size) {
      end = size;
    }

    path = path_cat(data_dir_opt, str_printf("fleas%d.db", i / 1000), NULL);

    Arr/*Json*/ *jsa = arr_new();
    RANGE(ifl, i, end) {
      arr_add(jsa, flea_serialize(fleas_get(fleas, ifl)));
    }_RANGE

    file_write(path, json_warray(jsa));
    i += 1000;
  }
}

static void save_bests (
  size_t cycle, Json *data, uint level, uint level10
) {
  if (cycle % level10) {
    return;
  }
  char *path = path_cat(data_dir_opt, str_printf("bests%d.db", level), NULL);

  Arr/*Json*/ *entry = arr_new();
  jarr_auint(entry, cycle);
  arr_add(entry, data);

  Json *entryjs = json_warray(entry);

  if (!file_exists(path)) {
    Arr/*Json*/ *datajs = arr_new();
    arr_add(datajs, entryjs);
    file_write(path, json_warray(datajs));
    return;
  }

  Json *file_data = file_read(path);

  Arr/*Json*/ *datajs = json_rarray(file_data);
  arr_insert(datajs, 0, entryjs);

  while (arr_size(datajs) > 10) {
    Json *extrajs = arr_get(datajs, 10);
    Arr/*Json*/ *extra_entryjs = json_rarray(extrajs);

    size_t c = json_ruint(arr_get(extra_entryjs, 0));
    save_bests(c, arr_get(extra_entryjs, 1), level + 1, level10 * 10);

    arr_remove(datajs, 10);
  }

  file_write(path, json_warray(datajs));
}

void io_save_bests(size_t cycle, Json *data) {
  save_bests(cycle, data, 1, 1);
}

void io_save_traces(Json *data) {
  char *path = path_cat(data_dir_opt, "traces.db", NULL);
  file_write(path, data);
}

void io_backup(char *dir) {
  if (!file_is_directory(dir)) {
    THROW "'%s' is not a directory", dir _THROW
  }

  char *path = dir;
  if (*dir != '/') {
    path = path_cat(file_cwd(), dir, NULL);
  }

  char *date = date_to_str(date_now());
  char *file = path_cat(path, str_printf("Fleas%s.zip", date), NULL);

  file_del(file);
  ext_zip(data_dir_base, file);
}

void io_restore(char *file) {
  ext_unzip(file, tmp_dir);

  char *new_fversion = path_cat(home, "tmp", "data", "version.txt", NULL);
  if (!file_exists(fversion)) {
    THROW
      "'%s' is not a Fleas backup:\n"
      "It does not contains the file 'data/version.txt'",
      file
    _THROW
  }

  char *version_data = file_read(new_fversion);
  if (strcmp(version_data, DATA_VERSION)) {
    Arr/*char*/ *parts_must = str_csplit_trim(DATA_VERSION, ':');
    Arr/*char*/ *parts_actual = str_csplit_trim(version_data, ':');
    THROW
      "'%s' is no a valid Fleas version backup:\n"
      "It has version '%s', but it ought to be '%s'",
      file, (char *)arr_get(parts_actual, 1), (char *)arr_get(parts_must, 1)
    _THROW
  }

  char *data_tmp = path_cat(home, "data2", NULL);
  char *data_new = path_cat(tmp_dir, "data", NULL);
  file_rename(data_dir_base, data_tmp);
  file_rename(data_new, data_dir_base);
  file_del(data_tmp);
}

void io_duplicates(void) {
  size_t fleaId;
  size_t cycle;
  Fleas *fleas;
  io_get_fleas(&fleaId, &cycle, &fleas);

  size_t n = 0;
  RANGE0(i, fleas_size(fleas) - 1) {
    Flea *f1 = fleas_get(fleas, i);
    RANGE(j, i + 1, fleas_size(fleas)) {
      if (flea_gen_eq(f1, fleas_get(fleas, j))) {
        fleas_set(fleas, j, flea_mutate(f1, fleaId++, cycle));
        ++n;
        break;
      }
    }_RANGE
  }_RANGE

  io_set_fleas(fleaId, cycle, fleas);
  printf("Removed %d fleas\n", n);
}

/*
void io_start(void) {
  file_write(path_cat(dataDir(), "start.lock", NULL), "");
}
*/
