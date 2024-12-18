// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constructors of built-in objects.

#ifndef OBJ_H
  #define OBJ_H

#include <pthread.h>
#include "kut/it.h"
#include "module.h"
#include "bfunction.h"
#include "typed/ttype.h"
#include "typed/tval.h"
#include "kut/bytes.h"
#include "kut/file.h"
#include "kut/tcp.h"

/// Creates an expression of the indicated object.
Exp *obj_module (Module *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Module *obj_get_module (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_module (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_bmodule (int value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int obj_get_bmodule (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_bmodule (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_bfunction (Bfunction value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Bfunction obj_get_bfunction (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_bfunction (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_iter (It *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
It *obj_get_iter (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_iter (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_bytes (Bytes *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Bytes *obj_get_bytes (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_bytes (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_file (File *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
File *obj_get_file (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_file (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_thread (pthread_t *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
pthread_t *obj_get_thread (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_thread (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_tcp_server (TcpServer *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
TcpServer *obj_get_tcp_server (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_tcp_server (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_tcp_conn (TcpConn *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
TcpConn *obj_get_tcp_conn (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_tcp_conn (Exp *this);

/// Creates an expression of typed object.
Exp *obj_typed (Ttype type, Tval value);

/// Read the type of a typed object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Ttype obj_get_typed_type (Exp *this);

/// Read the value of a typed object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tval obj_get_typed_value (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_typed (Exp *this);

#endif
