// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants.

#ifndef CTS_H
  #define CTS_H

/// Application name.
#define cts_app_name  "kutt"
/// Application version
#define cts_version "202503"
/// Data version.
#define cts_data_version "202503"
/// Data path.
char *cts_data_path (void);
/// Compilation data base path.
char *cts_compilation_path (void);
/// Temporary initialization base c-file.
char *cts_c_built_path (void);
/// Initialization base h-file.
char *cts_h_built_path (void);
/// Temporary initialization base o-file.
char *cts_o_built_path (void);
/// Initialization base library.
char *cts_a_built_path (void);
/// Returns a pointer to TRUE.
int *cts_true (void);
/// Returns a pointer to FALSE.
int *cts_false (void);

#endif
