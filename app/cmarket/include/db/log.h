// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Log table.

#ifndef DB_LOG_H
  #define DB_LOG_H

#define log_MSG(msg) log_msg(msg, __FILE__, (char *)__func__, __LINE__);

/// Initialize table.
void log_init ();

/// Adds file, function and line number to a message.
/// NOTE: This function is intended to be used internally. You should use log_MSG.
char *log_msg(char *message, char *file, char *func, int line);

/// Write an error message.
void log_error (char *msg);

/// Write an error warning.
void log_warning (char *msg);

/// Reset log.
void log_reset (char *msg);

/// Return log content in JSON format.
/// Log content is an array with each entry equals to:
///   isError: boolean. 'true' if the entri is an error.
///   date: char*. Date in format "%D/%M/%Y(%T)"
///   msg: char*. Entry message.
char *readJs (void);

#endif
