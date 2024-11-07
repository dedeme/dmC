// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "obj.h"
#include "exp.h"

#define module "<module>"
#define bmodule "<bmodule>"
#define bfunction "<bfunction>"
#define iter "<iter>"
#define bytes "<bytes>"
#define file "<file>"
#define thread "<thread>"
#define tcp_server "<tcpServer>"
#define tcp_conn "<tcpConn>"
#define typed "<typed>"

Exp *obj_module (Module *value) {
  return exp_object(module, value);
}

Module *obj_get_module (Exp *this) {
  return exp_get_object(module, this);
}

int obj_is_module (Exp *this) {
  return exp_is_object(module, this);
}

Exp *obj_bmodule (int value) {
  int *v = (int *)ATOMIC(sizeof(int));
  *v = value;
  return exp_object(bmodule, v);
}

int obj_get_bmodule (Exp *this) {
  int *v = exp_get_object(bmodule, this);
  return *v;
}

int obj_is_bmodule (Exp *this) {
  return exp_is_object(bmodule, this);
}

Exp *obj_bfunction (Bfunction value) {
  return exp_object(bfunction, value);
}

Bfunction obj_get_bfunction (Exp *this) {
  return exp_get_object(bfunction, this);
}

int obj_is_bfunction (Exp *this) {
  return exp_is_object(bfunction, this);
}

Exp *obj_iter (It *value) {
  return exp_object(iter, value);
}

It *obj_get_iter (Exp *this) {
  return exp_get_object(iter, this);
}

int obj_is_iter (Exp *this) {
  return exp_is_object(iter, this);
}

Exp *obj_bytes (Bytes *value) {
  return exp_object(bytes, value);
}

Bytes *obj_get_bytes (Exp *this) {
  return exp_get_object(bytes, this);
}

int obj_is_bytes (Exp *this) {
  return exp_is_object(bytes, this);
}

Exp *obj_file (File *value) {
  return exp_object(file, value);
}

File *obj_get_file (Exp *this) {
  return exp_get_object(file, this);
}

int obj_is_file (Exp *this) {
  return exp_is_object(file, this);
}

Exp *obj_thread (pthread_t *value) {
  return exp_object(thread, value);
}

pthread_t *obj_get_thread (Exp *this) {
  return exp_get_object(thread, this);
}

int obj_is_thread (Exp *this) {
  return exp_is_object(thread, this);
}

Exp *obj_tcp_server (TcpServer *value) {
  return exp_object(tcp_server, value);
}

TcpServer *obj_get_tcp_server (Exp *this) {
  return exp_get_object(tcp_server, this);
}

int obj_is_tcp_server (Exp *this) {
  return exp_is_object(tcp_server, this);
}

Exp *obj_tcp_conn (TcpConn *value) {
  return exp_object(tcp_conn, value);
}

TcpConn *obj_get_tcp_conn (Exp *this) {
  return exp_get_object(tcp_conn, this);
}

int obj_is_tcp_conn (Exp *this) {
  return exp_is_object(tcp_conn, this);
}

struct obj_Typed {
  Ttype type;
  Tval value;
};

Exp *obj_typed (Ttype type, Tval value) {
  struct obj_Typed *v = MALLOC(struct obj_Typed);
  v -> type = type;
  v -> value = value;
  return exp_object(typed, v);
}

Ttype obj_get_typed_type (Exp *this) {
  struct obj_Typed *v = exp_get_object(typed, this);
  return v->type;
}

Tval obj_get_typed_value (Exp *this) {
  struct obj_Typed *v = exp_get_object(typed, this);
  return v->value;
}

int obj_is_typed (Exp *this) {
  return exp_is_object(typed, this);
}

