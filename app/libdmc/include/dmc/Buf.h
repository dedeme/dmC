// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strings constructor

#ifndef DM_BUF_H
  #define DM_BUF_H

#include <stddef.h>

///
typedef struct buf_Buf Buf;

/// buf_new initializes a 'buf' with size 150 and length 0
Buf *buf_new (void);

/// buf_str returns the enveloped string
char *buf_str (Buf *this);

/// buf_length returns the length of the enveloped string
size_t buf_length (Buf *this);

/// buf_add_buf adds 'length bytes of 'data' to 'buf'.<br>
/// 'length' must be less or equals to 'strlen(data)'.<br>
/// It is not necessary that 'data' be a null-terminated string, but it must
/// no have characters \0
void buf_add_buf (Buf *this, char *data, size_t length);

/// buf_add adds 'data' to 'buf'.
void buf_add (Buf *this, char *data);

/// buf_cadd adds a character
void buf_cadd (Buf *this, char data);

/// buf_to_str returns a copy of the enveloped string
char *buf_to_str (Buf *this);

#endif
