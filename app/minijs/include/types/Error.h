// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Error data
/// <p>
/// Fields:
///   msg -> Error message
///   file -> File with error
///   nline -> line number of error (1 based)
///   nchar -> char number of error (0 based)
#ifndef TYPES_ERROR_H
  # define TYPES_ERROR_H

/*.-.*/

///
typedef struct error_Error Error;

///
Error *error_new(
  char *msg,
  char *file,
  int nline,
  int nchar
);

///
char *error_msg(Error *this);

///
char *error_file(Error *this);

///
int error_nline(Error *this);

///
int error_nchar(Error *this);

/*.-.*/

/// Returns a message kind of "Main.mini[1, 32]: A message".
char *error_fmsg(Error *this);


#endif
