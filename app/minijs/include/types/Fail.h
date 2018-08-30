// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Error data
/// <p>
/// Fields:
///   msg -> Error message
///   file -> File with error
///   nline -> line number of error (1 based)
///   nchar -> char number of error (0 based)
#ifndef TYPES_FAIL_H
  # define TYPES_FAIL_H

/*.-.*/

///
typedef struct fail_Fail Fail;

///
Fail *fail_new(
  char *msg,
  char *file,
  int nline,
  int nchar
);

///
char *fail_msg(Fail *this);

///
char *fail_file(Fail *this);

///
int fail_nline(Fail *this);

///
int fail_nchar(Fail *this);

/*.-.*/

/// Returns a message kind of "Main.mini[1, 32]: A message".
char *fail_fmsg(Fail *this);


#endif
