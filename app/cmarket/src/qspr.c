// Copyright 12-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "qspr.h"
#include "kut/DEFS.h"
#include "kut/rs.h"
#include "mkt/qs.h"
#include "data/cts.h"
#include "db/cosTb.h"
#include "db/quotesDb.h"
#include "db/log.h"

QsprRs *qspr_ok (void *value) {
  QsprRs *this = MALLOC(QsprRs);
  this->value = value;
  this->withError = FALSE;
  this->withWarnings = FALSE;
  return this;
}

QsprRs *qspr_error () {
  QsprRs *this = MALLOC(QsprRs);
  this->value = NULL;
  this->withError = TRUE;
  this->withWarnings = FALSE;
  return this;
}

QsprRs *qspr_warning (void *value) {
  QsprRs *this = qspr_ok(value);
  this->withWarnings = TRUE;
  return this;
}

QsprRs *qspr_bind (QsprRs *rs1, QsprRs *rs2) {
  return rs1->withError
    ? rs1
    : rs2->withError
      ? rs2
      : rs1->withWarnings
        ? qspr_warning(rs2->value)
        : rs2
  ;
}

// warns is Arr<char>
static void warnings_array(char *prefix_err_msg, Arr *warns) {
  EACH(warns, char, w) {
    log_warning(str_f("%s: %s", prefix_err_msg, w));
  }_EACH
}

// <Qs>
static QsprRs *toQs (char *prefix_err_msg, char *qs, int size) {
  // <char>
  Rs *rs = qs_from_str(size, qs);
  Qs *rqs = rs_get(rs);
  if (rqs) return qspr_ok(rqs);
  log_error (str_f("%s: %s", prefix_err_msg, rs_error(rs)));
  return qspr_error();
}

// <Qs>
static QsprRs *getMainQs (char *nick) {
  // <Qs>
  Rs *rs = quotesDb_read_qs(nick);
  Qs *rqs = rs_get(rs);
  if (rqs) return qspr_ok(rqs);
  log_error (str_f("Reading main company %s: %s", nick, rs_error(rs)));
  return qspr_error();
}

static QsprRs *simple_correction(char *prefix_err_msg, char *qs) {
  // <Qs>
  QsprRs *qqs = toQs(prefix_err_msg, qs, HISTORIC_QUOTES);
  if (qqs->withError) return qqs;
  // Tp<Qs, Arr<char>>
  Tp *tp = qs_correct(qqs->value);
  if (arr_size(tp_e2(tp))) {
    warnings_array (prefix_err_msg, tp_e2(tp));
    return qspr_warning(tp_e1(tp));
  } else {
    return qspr_ok(tp_e1(tp));
  }
}

static QsprRs *date_correction(char *prefix_err_msg, Qs *mqs, Qs *qs) {
    // Tp<Qs, Arr<char>>
    Tp *tp = qs_correct_dates(mqs, qs);
    if (arr_size(tp_e2(tp))) {
      warnings_array (prefix_err_msg, tp_e2(tp));
      return qspr_warning(tp_e1(tp));
    } else {
      return qspr_ok(tp_e1(tp));
    }
}

// <char>
QsprRs *qspr_correct_str (char *prefix_err_msg, char *qs) {
  // <Qs>
  QsprRs *qqs = simple_correction(prefix_err_msg, qs);
  if (qqs->withError) return qqs;
  if (qqs->withWarnings) return qspr_warning(qs_to_str(qqs->value));
  return qspr_ok(qs_to_str(qqs->value));
}

QsprRs *qspr_correct_str2 (char *prefix_err_msg, char *qs, char *main_nick) {
  // <Qs>
  QsprRs *qqs = simple_correction(prefix_err_msg, qs);
  if (qqs->withError) return qqs;

  // <Qs>
  QsprRs *mqqs = getMainQs(main_nick);
  if (mqqs->withError) return mqqs;
  // <Qs>
  QsprRs *qqs2 = date_correction(prefix_err_msg, mqqs-> value, qqs->value);
  if (qqs2->withError) return qqs2;

  qqs = qspr_bind(qqs, qspr_bind(mqqs, qqs2));

  if (qqs->withWarnings) return qspr_warning(qs_to_str(qqs->value));
  return qspr_ok(qs_to_str(qqs->value));
}
