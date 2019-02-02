// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "DEFS.h"
#include "tests.h"
#include "market/Quote.h"
#include "dmc/date.h"
#include "Flea.h"
#include "Fresults.h"
#include "Fbest.h"

static Quote **quotes = NULL;
static int qdays = 0;
static int qnicks = 0;

struct io_Flea_n {
  Fbest *f;
  int n;
};

static struct io_Flea_n *flea_n_new(Fbest *f, int n) {
  struct io_Flea_n *this = malloc(sizeof(struct io_Flea_n));
  this->f = f;
  this->n = n;
  return this;
}

static void flea_n_free(struct io_Flea_n *this) {
  free(this);
}

// fs is Arr[struct io_Flea_n]
void fleas_n_add(Arr *fs, Fbest *new_f) {
  int n = 1;
  EACH(fs, struct io_Flea_n, f)
    if (flea_eq_gen(
      fresults_flea(fbest_fresults(f->f)),
      fresults_flea(fbest_fresults(new_f))
    )) {
      n = f->n + 1;
    }
  _EACH
  arr_push(fs, flea_n_new(new_f, n));
}

// fs is Arr[struct io_Flea_n]
Fbest *fleas_n_select_new(Arr *fs) {
  if (!arr_size(fs))
    FAIL ("fs is empty");

  struct io_Flea_n *r = arr_get(fs, 0);
  EACH(fs, struct io_Flea_n, f)
    if (f->n >= r->n) {
      r = f;
    }
  _EACH
  return fbest_copy_new(r->f);
}

void io_init(void) {
  char *qdir = QUOTES_DATA_PATH;

  // Read nicks ------------------------------------------------------

  char *nicks_db = path_cat_new(qdir, "nicks.db", NULL);
  Js *js = (Js *)file_read_new(nicks_db);
  // Arr[Js]
  Arr *nicks_data = js_ra_new(js);
  char *nick_model_id = js_rs_new(arr_get(nicks_data, 1));
  // Arr[js]
  Arr *nicks_entries = js_ra_new(arr_get(nicks_data, 2));
  // Arr[char]
  Arr *nicks = arr_new(free);
  char *nick_model = NULL;
  EACH(nicks_entries, Js, js_entry)
    // Arr[Js]
    Arr *row = js_ra_new(js_entry);
    if (js_rb(arr_get(row, 3))) {
      arr_push(nicks, js_rs_new(arr_get(row, 1)));
      char *nick_id = js_rs_new(arr_get(row, 0));
      if (str_eq(nick_id, nick_model_id)) {
        nick_model = js_rs_new(arr_get(row, 1));
      }
      free(nick_id);
    }
    arr_free(row);
  _EACH
  if (!nick_model) {
    nick_model = str_new(arr_get(nicks, 0));
  }

  free(nicks_db);
  free(js);
  free(nick_model_id);
  arr_free(nicks_entries);
  arr_free(nicks_data);

  // Read days -------------------------------------------------------

  char *qmodelf = str_f_new("%s/quotes/%s.db", qdir, nick_model);
  qdays = 0;
  FileLck *lck = file_ropen(qmodelf);
  char *line = file_read_line_new(lck);
  while (*line) {
    ++qdays;
    free(line);
    line = file_read_line_new(lck);
  }
  file_close(lck);
  free(line);
  free(qmodelf);
  free(nick_model);

  // Rectify nicks ---------------------------------------------------

  int filter(char *nk) {
    char *qf = str_f_new("%s/quotes/%s.db", qdir, nk);
    FileLck *lck = file_ropen(qf);
    char *line = file_read_line_new(lck);
    int c = 0;
    while (*line) {
      ++c;
      free(line);
      line = file_read_line_new(lck);
    }
    free(line);
    file_close(lck);
    free(qf);
    return c == qdays;
  }
  arr_filter(nicks, (FPRED)filter);
  arr_sort(nicks, (FGREATER)str_greater);
  qnicks = arr_size(nicks);

  // Read quotes -----------------------------------------------------

  quotes = malloc(qnicks * qdays * sizeof(Quote *));
  int col = 0;
  EACH(nicks, char, nk)
    // Varr[Quote]
    Varr *qs = varr_new();
    char *qf = str_f_new("%s/quotes/%s.db", qdir, nk);
    FileLck *lck = file_ropen(qf);
    char *line = file_read_line_new(lck);
    while (*line) {
      // Arr[char]
      Arr *parts = str_csplit_trim_new(line, ':');
      varr_push(qs, quote_new(
        atof(arr_get(parts, 1)),
        atof(arr_get(parts, 2)),
        atof(arr_get(parts, 3)),
        atof(arr_get(parts, 4)),
        atoi(arr_get(parts, 5))
      ));
      arr_free(parts);
      free(line);
      line = file_read_line_new(lck);
    }

    if (varr_size(qs) != qdays) {
      FAIL(str_f_new(
        "Wrong quotes number of '%s'. It has %d but shuld have %d",
        nk, varr_size(qs), qdays
      ));
    }

    varr_reverse(qs);

    int row = col;
    EACH(qs, Quote, q)
      quotes[row] = q;
      row += qnicks;
    _EACH
    ++col;

    file_close(lck);
    free(line);
    free(qf);
    varr_free(qs);
  _EACH

  // --------------------------------------------------- //
  // tests_list_nicks(nicks, nick_model, qdays, qnicks); // Debug
  // --------------------------------------------------- //

  arr_free(nicks);

  // Write configuration ---------------------------------------------
  char *fconf = path_cat_new(sys_home(), "conf.db", NULL);
  // Map[Js]
  Map *m = map_new(free);
  map_put(m, "max_days", js_wi_new(MAX_DAYS));
  map_put(m, "min_days", js_wi_new(MIN_DAYS));
  map_put(m, "max_strip", js_wd_new(MAX_STRIP, 4));
  js = js_wo_new(m);
  file_write(fconf, (char *)js);

  free(fconf);
  map_free(m);
  free(js);
}

void io_end(void) {
  Quote **p = quotes;
  REPEAT(qdays * qnicks)
    quote_free(*p++);
  _REPEAT
  free(quotes);
}

Quote **io_quotes(void) {
  return quotes;
}

int io_qdays(void) {
  return qdays;
}

int io_qnicks(void) {
  return qnicks;
}

// Read fleas --------------------------------------------------------

void io_write_fleas(
  char *days,
  int cycle,
  // Arr[Flea]
  Arr *fleas
) {
  // Arr[Js]
  Arr *data = arr_new(free);
  arr_push(data, js_wi_new(cycle));
  arr_push(data, arr_to_js_new(fleas, (FTO)flea_to_js_new));
  Js *js = js_wa_new(data);

  char *f = str_f_new("%s/%s.db", sys_home(), days);
  file_write(f, (char*)js);

  arr_free(data);
  free(js);
  free(f);
}

void io_read_fleas(
  int *cycle,
  // Arr[Flea]
  Arr **fleas,
  char *days
) {
  char *f = str_f_new("%s/%s.db", sys_home(), days);
  if (file_exists(f)) {
    Js *js = (Js *)file_read_new(f);
    // Arr[Js]
    Arr *data = js_ra_new(js);
    *cycle = js_ri(arr_get(data, 0));
    *fleas = arr_from_js_new(
      arr_get(data, 1),
      (FFROM)flea_from_js_new,
      (FPROC)flea_free
    );
    free(f);
    free(js);
    arr_free(data);
    return;
  }
  *cycle = 0;
  *fleas = arr_new((FPROC)flea_free);
  free(f);
}

// Write results -----------------------------------------------------

void io_write_results(char *group, Arr *results) {
  char *dir = path_cat_new(sys_home(), group, NULL);
  file_mkdir(dir);
  time_t tm = date_now();
  char *date = date_to_str_new(tm);
  char *f = path_cat_new(dir, date, NULL);

  Js *js = arr_to_js_new(results, (FTO)fresults_to_js_new);
  file_write(f, (char *)js);

  free(dir);
  free(date);
  free(f);
  free(js);
}

// Best process ------------------------------------------------------

void io_best_start(
  Arr **rbests,
  char **date,
  // Arr[Fresults]
  Arr **results,
  int days
) {
  char *group = str_f_new("%d", days);

  // get bests -----------------------------------
  // Arr[Fbest]
  Arr *bests;

  char *bpath = str_f_new("%s/bests/%s.db", sys_home(), group);
  if (file_exists(bpath)) {
    Js *js = (Js *)file_read_new(bpath);
    bests = arr_from_js_new(js, (FFROM)fbest_from_js_new, (FPROC)fbest_free);
    free(js);
  } else {
    char *path = path_cat_new(sys_home(), "bests", NULL);
    file_mkdir(path);
    bests = arr_new((FPROC)fbest_free);
    free(path);
  }

  // get dates -----------------------------------
  // Arr[char]
  Arr *dates;

  char *path = path_cat_new(sys_home(), group, NULL);
  dates = file_dir_new(path);
  arr_sort(dates, (FGREATER)str_greater);

  free(path);

  // setup bests ---------------------------------
  int filter (void *best) {
    Fbest *b = (Fbest *)best;
    char *bdate = fbest_date(b);

    int index (void *date) {
      char *d = (char *)date;
      return str_eq(d, bdate);
    }
    return arr_index(dates, index) != -1;
  }
  arr_filter(bests, filter);

  // Remove historic -----------------------------

  // set date, remove hitoric, and set last results -

  *date = str_new(arr_get(dates, arr_size(dates) - 1));

  // set and write bests

  // Arr[Fbest]
  Arr *new_bests = arr_new((FPROC)fbest_free);
  int bsize = arr_size(bests);
  if (bsize == 0) {
    Fbest *b = fbest_new(
      str_new("20000101"),
      fresults_new(flea_new(str_new("0")), 0, 0, 0),
      0, 1000000
    );
    arr_push(new_bests, b);
  } else {
    char *current_month = str_new(*date);
    str_left(&current_month, 6);
    char *last_month = str_new("");
    // Arr [struct io_Flea_n]
    Arr *fleas = arr_new((FPROC)flea_n_free);
    EACH(bests, Fbest, b)
      char *month = str_new(fbest_date(b));
      str_left(&month, 6);

      if (str_eq(month, last_month)) {
        if (str_eq(month, current_month)) {
          arr_push(new_bests, fbest_copy_new(b));
        } else {
          fleas_n_add(fleas, b);
        }
      } else if (str_eq(month, current_month)) {
        if (*last_month) {
          arr_push(new_bests, fleas_n_select_new(fleas));
          arr_free(fleas);
          fleas = arr_new((FPROC)flea_n_free);
        }
        free(last_month);
        last_month = str_new(month);

        arr_push(new_bests, fbest_copy_new(b));
      } else {
        if (*last_month) {
          arr_push(new_bests, fleas_n_select_new(fleas));
          arr_free(fleas);
          fleas = arr_new((FPROC)flea_n_free);
        }
        free(last_month);
        last_month = str_new(month);

        fleas_n_add(fleas, b);
      }
      free(month);
    _EACH

    if (arr_size(fleas)) {
      arr_push(new_bests, fleas_n_select_new(fleas));
    }

    free(current_month);
    free(last_month);
    arr_free(fleas);
  }
  arr_free(bests);

  *rbests = new_bests;

  Js *js = arr_to_js_new(new_bests, (FTO)fbest_to_js_new);
  file_write(bpath, (char *)js);
  free(js);
  free (bpath);

  // setup group ---------------------------------

  EACH(dates, char, date)
    int del = 1;
    EACH(new_bests, Fbest, b)
      if (str_eq(date, fbest_date(b))) {
        del = 0;
      }
    _EACH
    if (del) {
      char *path = path_cat_new(sys_home(), group, date, NULL);
      file_del(path);
      free(path);
    }
  _EACH

  arr_free(dates);

  // set results ---------------------------------

  path = path_cat_new(sys_home(), group, *date, NULL);
  js = (Js *)file_read_new(path);
  *results = arr_from_js_new(
    js, (FFROM)fresults_from_js_new, (FPROC)fresults_free
  );
  free(js);
  free(path);
  free(group);
}

void io_write_best(int days, Fbest *best) {
  best = fbest_copy_new(best);
  char *group = str_f_new("%d", days);
  char *path = str_f_new("%s/bests/%s.db", sys_home(), group);
  Js *js = (Js *)file_read_new(path);
  // Arr[Fbest]
  Arr *bests = arr_from_js_new(js, (FFROM)fbest_from_js_new, (FPROC)fbest_free);

  int size1 = arr_size(bests) - 1;
  Fbest *last = NULL;
  if (size1 >= 0) {
    last = arr_get(bests, size1);
  }
  if (last && str_eq(fbest_date(last), fbest_date(best))) {
    arr_set(bests, size1, best);
  } else {
    arr_push(bests, best);
  }

  Js *js2 = arr_to_js_new(bests, (FTO)fbest_to_js_new);
  file_write(path, (char *)js2);

  free(js2);
  arr_free(bests);
  free(js);
  free(path);
  free(group);
}

// App management ----------------------------------------------------

int io_lock(void) {
  char *f = path_cat_new(sys_home(), "lock", NULL);
  if (file_exists(f)){
    puts("fleas already is running.\nOtherwise type:\nfleas unlock\n");
    return 1;
  }
  file_write(f, "");
  free(f);
  return 0;
}

void io_unlock(void) {
  char *f = path_cat_new(sys_home(), "lock", NULL);
  file_del(f);
  free(f);
}

void io_send_stop(void) {
  char *f = path_cat_new(sys_home(), "stop", NULL);
  file_write(f, "");
  free(f);
}

int io_stop(void) {
  char *f = path_cat_new(sys_home(), "stop", NULL);
  int r = file_exists(f);
  free(f);
  return r;
}

void io_del_stop(void) {
  char *f = path_cat_new(sys_home(), "stop", NULL);
  file_del(f);
  free(f);
}

void io_clean(void) {
  // Arr[char]
  Arr *fs = file_dir_new(sys_home());
  EACH(fs, char, f)
    char *path = path_cat_new(sys_home(), f, NULL);
    if (!file_is_directory(path) && !str_eq(f, "conf.db")) {
      file_del(path);
    }
    free(path);
  _EACH
  arr_free(fs);
}

