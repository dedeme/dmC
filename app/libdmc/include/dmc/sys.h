// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing globals values.<br>
/// A way to initializes system is:
///   sys_new("app_name");
///   sys_locale("es_ES.utf8");
///   sys_random();
/// If application is a 'web application' you can sustitute
/// <tt>sys_new("app_name")</tt> by <tt>sys_new_cgi("app_name")</tt>

#ifndef DM_SYS_H
  #define DM_SYS_H

typedef struct achar_Achar Achar;

/// Initializates a normal program and call 'exc.init()' and 'rnd_init()'.<br>
/// After call this function 'sys_home' and 'sys_user' are available.<p>
/// Creates the user directory in "~/.dmCApp/" + 'path'
void sys_init (char *path);

///
char *sys_home (void);

///
char *sys_uname (void);

/// Sets LC_ALL, for example: <tt>sys_locale("es_ES.utf8")</tt>
void sys_locale (char *language);

/// Executes 'command', redirecting stderr to stdout, and returns its standard
/// out (excluding the trailing '\n' of each line). If command fails, function
/// returns NULL.
Achar *sys_cmd(char *command);

#endif
