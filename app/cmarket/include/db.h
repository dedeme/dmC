// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Data base management.

#ifndef DB_H
  #define DB_H

/// Initialize data base.
void db_init (void);

/// Lock data base.
/// Intended to be used by 'server_stop'
/// Returns 1 if the data base could be locked. Otherwise returns 0.
int db_lock (void);

/// Unlock data base.
/// Intended to be used by 'server_stop'
void db_unlock (void);

/// Read a table.
char *db_read (char *path);

/// Write text in table
void db_write (char *path, char *text);

#endif
