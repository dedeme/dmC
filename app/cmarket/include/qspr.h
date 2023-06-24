// Copyright 12-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quotes management processes.

#ifndef QSPR_H
  #define QSPR_H

///
typedef struct qspr_QsprRs {
  int withWarnings;
  int withError;
  void *value;
} QsprRs;

/// Create a result without error nor warnings.
QsprRs *qspr_ok (void *value);

/// Create a result with error written in log. (QsprRs->value = NULL)
QsprRs *qspr_error (void);

/// Create a result with warnings written in log.
QsprRs *qspr_warning (void *value);

/// Returns a new QsprRs such that:
///   if rs1->withError == TRUE -> ret->withError = TRUE & ret->value = NULL.
///   if rs2->withError == TRUE -> retrun rs2.
///   otherwise -> ret->withError = FALSE, ret -> rs2->value.
///                ret->withWarning = rs1->withWarning || rs2->withWarning.
QsprRs *qspr_bind (QsprRs *rs1, QsprRs *rs2);

/// Return an QsprRs<char> with 'qs' corected.
/// Return and paremeter quotes are type file 'NICK.tb'.
///   nick: Company nick with quotes 'qs'.
///   qs: Copany quotes. (Company can not be main)
QsprRs *qspr_correct_str (char *prefix_err_msg, char *qs);

/// Return an QsprRs<char> with 'qs' corected.
/// Return and paremeter quotes are type file 'NICK.tb'.
///   nick: Company nick with quotes 'qs'.
///   qs: Copany quotes.
///   main_nick: Main company nick.
QsprRs *qspr_correct_str2 (char *prefix_err_msg, char *qs, char *main_nick);

/// Return an QsprRs<char> with saved nick quotes corrected.
/// Return are type file 'NICK.tb'.
QsprRs *qspr_correct_nick (char *main_nick, char *nick);

/// Downloads quotes from historic of 'nick' and merges them with the saved ones.
/// Returns an QsprRs<char>.
///   nick: Company to download.
QsprRs *qspr_download2 (char *main_nick, char *nick);

/// Downloads quotes from historic of 'nick' and merges them with the saved ones.
/// Returns an QsprRs<char>.
///   nick: Company to download. (Company can not be main).
QsprRs *qspr_download (char *nick);

#endif
