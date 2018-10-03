// Copyright 26-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "DEFS.h"
#include "Flea.h"
#include "data/HistoricEntry.h"
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
#include "dmc/ct/Mjson.h"

static char *cat(char *base, char *dir) {
  return path_cat(base, dir, NULL);
}

void io_init(void) {
  char *home = sys_home();
  file_mkdir(home);

  Group **groups;
  int groups_number;
  io_groups_list(&groups, &groups_number);
  RANGE0(i, groups_number) {
    char *sel_type = io_group_to_str(groups[i]);

    char *sel_dir = cat(home, sel_type);
    file_mkdir(sel_dir);

    char *historic = cat(sel_dir, "historic");
    file_mkdir(historic);
  }_RANGE
}

void io_clean(void) {
  Group **groups;
  int groups_number;
  io_groups_list(&groups, &groups_number);
  RANGE0(i, groups_number) {
    char *sel_type = io_group_to_str(groups[i]);

    char *sel_dir = cat(sys_home(), sel_type);
    file_del(sel_dir);
    file_mkdir(sel_dir);

    char *historic = cat(sel_dir, "historic");
    file_mkdir(historic);
  }_RANGE
}

bool io_lock(void) {
  char *lock = path_cat(sys_home(), "lock", NULL);
  if (file_exists(lock)) {
    puts("Fleas already is running.\nOtherwise type:\nfleas unlock\n");
    return false;
  }
  file_write(lock, "");
  return true;
}

void io_unlock(void) {
  char *lock = path_cat(sys_home(), "lock", NULL);
  file_del(lock);
}

void io_send_stop(void) {
  char *stop = path_cat(sys_home(), "stop", NULL);
  file_write(stop, "");
}

void io_remove_stop(void) {
  char *stop = path_cat(sys_home(), "stop", NULL);
  file_del(stop);
}

bool io_stoped(void) {
  char *stop = path_cat(sys_home(), "stop", NULL);
  return file_exists(stop);
}

static char *gs[3] = {"sel", "ibex", "all"};
void io_groups_list(Group ***groups, int *groups_number) {
  *groups = (Group **)gs;
  *groups_number = 3;
}

char *io_group_to_str(Group *group) {
  return (char *)group;
}

// If date does not match with q.date, returns NULL.
static Quote *quote_from_str(char *date, char *q) {
  Achar *parts = str_csplit(q, ':');
  if (str_eq(achar_get(parts, 0), date)) {
    return quote_new(
      atof(achar_get(parts, 1)),
      atof(achar_get(parts, 2)),
      atof(achar_get(parts, 3)),
      atof(achar_get(parts, 4)),
      atoi(achar_get(parts, 5))
    );
  }
  return NULL;
}

static Achar *read_dates(char *model_path) {
  Achar *r = achar_new();
  char *qs = str_trim(file_read(model_path));
  EACH(str_csplit(qs, '\n'), char, q) {
    Achar *parts = str_csplit(q, ':');
    achar_add(r, achar_get(parts, 0));
  }_EACH
  achar_reverse(r);
  return r;
}

void io_read_nicks(
  int *model,
  Achar **nicks,
  char *data_dir,
  Group *sel
) {
  char *version = file_read(path_cat(data_dir, "version.txt", NULL));
  if (!str_eq(version, DATA_VERSION)) {
    exc_illegal_state(str_printf(
      "Data base version '%s' is expected, but '%s' was found",
      DATA_VERSION, version
    ));
  }
  Json *js = (Json *)file_read(path_cat(data_dir, "nicks.db", NULL));
  Ajson *data = json_rarray(js);
  char *model_id = json_rstring(ajson_get(data, 1));
  Ajson *nicks_js = json_rarray(ajson_get(data, 2));
  Achar *nks = achar_new();
  char *model_nick = "";
  EACH(nicks_js, Json, js) {
    Ajson *nk_js = json_rarray(js);
    if (
      str_eq(io_group_to_str(sel), "ibex") &&
      !json_rbool(ajson_get(nk_js, 2))
    ) {
      continue;
    }
    if (
      str_eq(io_group_to_str(sel), "sel") &&
      !json_rbool(ajson_get(nk_js, 3))
    ) {
      continue;
    }
    char *nk = json_rstring(ajson_get(nk_js, 1));
    if (str_eq(json_rstring(ajson_get(nk_js, 0)), model_id)) {
      model_nick = nk;
    }
    achar_add(nks, nk);
  }_EACH
  achar_sort(nks);

  int md = -1;
  EACH(nks, char, nk) {
    if (str_eq(nk, model_nick)) {
      md = _i;
      break;
    }
  }_EACH
  if (md == -1) {
    md = 0;
  }

  *model = md;
  *nicks = nks;
}

void io_read_quotes(
  Achar **nicks,
  Achar **dates,
  Quote ****quotes,
  char *data_dir,
  Group *sel
) {
  int model;
  Achar *nks;
  io_read_nicks(&model, &nks, data_dir, sel);

  char *q_dir = path_cat(data_dir, "quotes", NULL);

  Achar *dts = read_dates(str_printf("%s/%s.db", q_dir, achar_get(nks, model)));
  int dates_size = achar_size(dts);

  Achar *rnicks = achar_new();
  Aaquote *qs = aaquote_new();
  REPEAT(dates_size) {
    aaquote_add(qs, aquote_new());
  }_REPEAT

  EACH(nks, char, nk) {
    char *n_path = str_printf("%s/%s.db", q_dir, nk);
    char *data = str_trim(file_read(n_path));
    Achar *qs_str = str_csplit(data, '\n');
    if (achar_size(qs_str) != dates_size) {
      continue;
    }
    achar_reverse(qs_str);

    Aquote *qs_nick = aquote_new();
    EACH(qs_str, char, q_str) {
      Quote *q = quote_from_str(achar_get(dts, _i), q_str);
      if (q) {
        aquote_add(qs_nick, q);
      } else {
        break;
      }
    }_EACH
    if (aquote_size(qs_nick) != dates_size) {
      continue;
    }

    RANGE0(i, dates_size) {
      aquote_add(aaquote_get(qs, i), aquote_get(qs_nick, i));
    }_RANGE
    achar_add(rnicks, nk);
  }_EACH

  if (achar_size(nks) - achar_size(rnicks) > 20) {
    THROW("fleas")
      exc_illegal_state("Reading has been fail in more than 20 companies")
    _THROW
  }

  int nicks_size = achar_size(rnicks);
  Quote ***rquotes = GC_MALLOC(dates_size * sizeof(Quote **));
  RANGE0(row, dates_size) {
    Quote **day = GC_MALLOC(nicks_size * sizeof(Quote *));
    rquotes[row] = day;
    Aquote *dayq = aaquote_get(qs, row);
    RANGE0(col, nicks_size) {
      Quote *r = aquote_get(dayq, col);
      day[col] = r;
    }_RANGE
  }_RANGE

  *nicks = rnicks;
  *dates = dts;
  *quotes = rquotes;
}

void io_write_family(
  Group *gr,
  Family *fm,
  int cycle,
  double cut_proximity,
  double inc_proximity,
  Aflea *fleas
){
  Mjson *mjs = mjson_new();
  jmap_pint(mjs, "cycle", cycle);
  jmap_pdouble(mjs, "cut_proximity", cut_proximity, 8);
  jmap_pdouble(mjs, "inc_proximity", inc_proximity, 8);
  jmap_parray(mjs, "fleas", (Arr *)fleas, (Json *(*)(void *))flea_to_json);

  char *path = str_printf(
    "%s/%s/%s.db", sys_home(), io_group_to_str(gr), flea_family_to_str(fm)
  );
  file_write(path, (char *)json_wobject(mjs));
}

void io_read_family(
  int *cycle,
  double *cut_proximity,
  double *inc_proximity,
  Aflea **fleas,
  Group *gr,
  Family *fm
) {
  *cycle = 0;
  *cut_proximity = 0.5;
  *inc_proximity = 0.5;
  *fleas = aflea_new();

  char *path = str_printf(
    "%s/%s/%s.db", sys_home(), io_group_to_str(gr), flea_family_to_str(fm)
  );

  if (file_exists(path)) {
    Json *js = (Json *)file_read(path);
    Mjson *mjs = json_robject(js);
    *cycle = jmap_gint(mjs, "cycle");
    *cut_proximity = jmap_gdouble(mjs, "cut_proximity");
    *inc_proximity = jmap_gdouble(mjs, "inc_proximity");
    *fleas =
      (Aflea *)jmap_garray(mjs, "fleas", (void *(*)(Json *))flea_from_json);
  }
}

void io_write_historic(Group *gr, int cycle, Arr/*HistoricEntry*/ *historic) {
  char *dir = path_cat(sys_home(), io_group_to_str(gr), "historic", NULL);
  char *path = str_printf("%s/h%d", dir, cycle);

  Ajson *ajs = ajson_new();
  EACH(historic, HistoricEntry, e) {
    ajson_add(ajs, historicEntry_to_json(e));
  }_EACH
  file_write(path, (char *)json_warray(ajs));

  // Clean
  EACH(file_dir(dir), char, f) {
    char *fname = path_name(f);
    int fcycle = atoi(str_sub_end(fname, 1));
    int cut = 10000;
    while (cut > 9) {
      if (cycle - fcycle > cut && fcycle % cut) {
        file_del(f);
      }
      cut /= 10;
    }
  }_EACH
}

///
Map/*Arr[HistoricEntry]*/ *io_read_historic(Group *gr) {
  char *dir = path_cat(sys_home(), io_group_to_str(gr), "historic", NULL);
  Map/*Arr[HistoricEntry]*/ *r = map_new();
  EACH(file_dir(dir), char, f) {
    char *fname = path_name(f);
    char *fcycle = str_sub_end(fname, 1);

    Ajson *js = json_rarray((Json *)file_read(f));
    Arr/*HistoricEntry*/ *a = arr_new();
    EACH(js, Json, j) {
      arr_add(a, historicEntry_from_json(j));
    }_EACH

    map_put(r, fcycle, a);
  }_EACH
  return r;
}
