// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "dmc/date.h"
#include "DEFS.h"
#include "Model.h"
#include "Rs.h"

static int qdays = 0;
static int qnicks = 0;
static double *opens = NULL;
static double *closes = NULL;
// Arr[char]
static Arr *nicks = NULL;
// Arr[char]
static Arr *dates = NULL;

static double read_double(char *s) {
  int errno = 0;
  char *tail;
  double r = strtod(s, &tail);
  if (errno || *tail) {
    return -1;
  }
  return r;
}

static char *read_nick_new(char *nick) {
  char *f = str_f_new("%s/quotes/%s.db", QUOTES_DATA_PATH, nick);
  char *data = file_read_new(f);
  free(f);
  return data;
}

static int nlines(char *nick) {
  char *data = read_nick_new(nick);
  // Arr[char]
  Arr *lines = str_csplit_new(data, '\n');
  int c = 0;
  EACH(lines, char, l)
    char *ltrim = str_new(l);
    str_trim(&ltrim);
    if (*ltrim) {
      ++c;
    }
    free(ltrim);
  _EACH
  arr_free(lines);
  free(data);
  return c;
}

// Arr[Model]
void io_init(Arr *fmodels) {

  // Read nicks and nicks model -----------------------------------------------
  char *nicks_db = path_cat_new(QUOTES_DATA_PATH, "nicks.db", NULL);
  char *nicks_data = file_read_new(nicks_db);
  free(nicks_db);
  // Arr[Js]
  Arr *nicks_data_js = js_ra_new((Js *)nicks_data);
  free(nicks_data);

  char *nick_model_id = js_rs_new(arr_get(nicks_data_js, 1));
  // Arr[js]
  Arr *nicks_js = js_ra_new(arr_get(nicks_data_js, 2));
  arr_free(nicks_data_js);

  nicks = arr_new(free);
  char *nick_model = NULL;
  EACH(nicks_js, Js, nk_data_js)
    // Arr[Js]
    Arr *nk_js = js_ra_new(nk_data_js);
    if (js_rb(arr_get(nk_js, 3))) {
      char *nk = js_rs_new(arr_get(nk_js, 1));
      arr_push(nicks, nk);
      char *id = js_rs_new(arr_get(nk_js, 0));
      if (str_eq(id, nick_model_id)) {
        nick_model = str_new(nk);
      }
      free(id);
    }
    arr_free(nk_js);
  _EACH
  free(nick_model_id);
  arr_free(nicks_js);

  if (!nick_model) {
    nick_model = str_new(arr_get(nicks, 0));
  }

  // Read qdays ---------------------------------------------------------------
  qdays = nlines(nick_model);

  // Remove wrong nicks and read qnicks ---------------------------------------
  int ffilter (char *nk) {
    return nlines(nk) == qdays;
  }
  arr_filter(nicks, (FPRED)ffilter);
  qnicks = arr_size(nicks);

  // Read quotes and dates ----------------------------------------------------
  dates = arr_new(free);

  int dim = qdays * qnicks;
  opens = malloc(dim * sizeof(double));
  closes = malloc(dim * sizeof(double));

  EACH_IX(nicks, char, nk, i)
    int c = dim - qnicks + i;
    char *data = read_nick_new(nk);
    // Arr[char]
    Arr *lines = str_csplit_new(data, '\n');

    EACH(lines, char, l)
      char *ltrim = str_new(l);
      str_trim(&ltrim);
      if (*ltrim) {
        // Arr[char]
        Arr *qs = str_csplit_new(ltrim, ':');
        if (!i) { // Loads dates
          arr_push(dates, str_new(arr_get(qs, 0)));
        }
        opens[c] = read_double(arr_get(qs, 1));
        closes[c] = read_double(arr_get(qs, 2));
        c -= qnicks;
        arr_free(qs);
      }
      free(ltrim);
    _EACH

    arr_free(lines);
    free(data);
  _EACH

  arr_reverse(dates);

/* Tests
 {
    printf("Nick model: %s\nqdays: %d\nqnicks: %d\n",
      nick_model, qdays, qnicks);
    int i = 0;
    EACH_IX(nicks, char, nick, ix)
      if (str_eq(nick, "TEF")) {
        i = ix;
      }
      printf("%s ", nick);
    _EACH
    puts("");
    REPEAT(qdays)
      printf("%f - %f\n", opens[i], closes[i]);
      i += qnicks;
    _REPEAT
  }
*/

  // Application data ---------------------------------------------------------
  char *bests_dir = str_f_new("%s/_bests", sys_home());
  file_mkdir(bests_dir);
  char *charts_dir = str_f_new("%s/_charts", sys_home());
  file_mkdir(charts_dir);
  EACH(fmodels, Model, m)
    char *name = model_name(m);
    char *model_dir = str_f_new("%s/%s", sys_home(), name);
    file_mkdir(model_dir);
    char *conf = str_f_new("%s/conf.db", model_dir);
    if (!file_exists(conf)) {
      // Arr[Js]
      Arr *names_js = arr_new(free);
      EACH(model_param_names(m), char, n)
        arr_push(names_js, js_ws_new(n));
      _EACH
      Js *names = js_wa_new(names_js);

      // Arr[Js]
      Varr *data = varr_new();
      varr_push(data, names);
      varr_push(data, model_param_jss(m));

      Js *js = js_wa_new((Arr *)data);
      file_write(conf, (char *)js);

      arr_free(names_js);
      free(names);
      varr_free(data);
      free(js);
    }
    char *bests_f = str_f_new("%s/%s.db", bests_dir, name);
    if (!file_exists(bests_f)) {
      file_write(bests_f, "[]");
    }
    free(bests_f);
    free(conf);
    free(model_dir);
  _EACH
  free(bests_dir);
  free(charts_dir);

  free(nick_model);
}

void io_free(void) {
  free(closes);
  free(opens);
  arr_free(nicks);
  arr_free(dates);
}

int io_qnicks(void) {
  return qnicks;
}

int io_qdays(void) {
  return qdays;
}

double *io_opens(void) {
  return opens;
}

double *io_closes(void) {
  return closes;
}

Arr *io_nicks(void) {
  return nicks;
}

Arr *io_dates(void) {
  return dates;
}

Arr *io_rrss_new(char *model) {
  char  *f = str_f_new("%s/_bests/%s.db", sys_home(), model);
  char *data = file_read_new(f);
  // Arr[Js]
  Arr *jss = js_ra_new((Js *)data);
  // Arr[BestsRs]
  Arr *r = arr_new((FPROC)rs_free);
  EACH(jss, Js, js)
    arr_push(r, rs_from_js_new(js));
  _EACH
  free(f);
  free(data);
  arr_free(jss);
  return r;
}

void io_wrss(int is_bests, char *model, Arr *rss) {
  char *create_path_new() {
    if (is_bests) {
      return str_f_new("%s/_bests/%s.db", sys_home(), model);
    }
    char *date = date_to_str_new(date_now());
    char *r = str_f_new("%s/%s/%s", sys_home(), model, date);
    free(date);
    return r;
  }
  char  *f = create_path_new();
  // Arr[Js]
  Arr *jss = arr_new(free);
  EACH(rss, Rs, rs)
    arr_push(jss, rs_to_js_new(rs));
  _EACH
  Js *data = js_wa_new(jss);
  file_write(f, (char *)data);
  free(f);
  arr_free(jss);
  free(data);
}

void io_wcharts(char *model, char *data) {
  char *f = str_f_new("%s/_charts/%s.db", sys_home(), model);
  file_write(f, data);
  free(f);
}
