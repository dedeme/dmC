// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// IO operations.

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"

/// Intializes data base.
void io_init(void);

/// Writes a new lock
void io_lock_write(char *key);

/// Deletes lock
void io_lock_del(void);

/// Tests if 'key' is correct
int io_lock_check(char *key);

/// Writes a log error
void io_loge(const char *format, ...);

/// Writes a log warning
void io_logw(const char *format, ...);

/// Writes state
void io_state_write(char *state);

/// Reads state
void io_state_read(char **state_new);

/// Writes servers list to pick up
///   box (Arr[char]): List to pick up
void io_servers_write(Arr *box);

/// Reads servers list to pick up. If there are no servers return an empty
/// Array.
///   Returns Arr[char]
Arr *io_servers_read_new(void);

/// Writes the current server name
void io_server_current_write(char *server_name);

/// Reads the current server name
char *io_server_current_read_new(void);

/// Writes server 'id' data "jsonized"
void io_server_write(char *id, char *js);

/// Reads server 'id' data "jsonized"
char *io_server_read_new(char *id);

/// Writes nick data  'jsonized'
void io_nick_write(char *nick, char *data);

/// Reads nick data 'jsonized'
char *io_nick_read_new(char *nick);

/// Returns the selected nicks (Arr[char] -> ["ANA, BBVA, ..."])
Arr *io_nicks_new(void);

/// Deletes data of nicks quotes
void io_nicks_clear(void);

#endif
