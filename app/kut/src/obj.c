// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

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

Exp *obj_module (Module *value) {
  return exp_object(module, value);
}

Module *obj_rget_module (Exp *this) {
  return exp_rget_object(module, this);
}

int obj_is_module (Exp *this) {
  return exp_is_object(module, this);
}

Exp *obj_bmodule (char *value) {
  return exp_object(bmodule, value);
}

char *obj_rget_bmodule (Exp *this) {
  return exp_rget_object(bmodule, this);
}

int obj_is_bmodule (Exp *this) {
  return exp_is_object(bmodule, this);
}

Exp *obj_bfunction (Bfunction value) {
  return exp_object(bfunction, value);
}

Bfunction obj_rget_bfunction (Exp *this) {
  return exp_rget_object(bfunction, this);
}

int obj_is_bfunction (Exp *this) {
  return exp_is_object(bfunction, this);
}

Exp *obj_iter (It *value) {
  return exp_object(iter, value);
}

It *obj_rget_iter (Exp *this) {
  return exp_rget_object(iter, this);
}

int obj_is_iter (Exp *this) {
  return exp_is_object(iter, this);
}

Exp *obj_bytes (Bytes *value) {
  return exp_object(bytes, value);
}

Bytes *obj_rget_bytes (Exp *this) {
  return exp_rget_object(bytes, this);
}

int obj_is_bytes (Exp *this) {
  return exp_is_object(bytes, this);
}

Exp *obj_file (File *value) {
  return exp_object(file, value);
}

File *obj_rget_file (Exp *this) {
  return exp_rget_object(file, this);
}

int obj_is_file (Exp *this) {
  return exp_is_object(file, this);
}

Exp *obj_thread (pthread_t *value) {
  return exp_object(thread, value);
}

pthread_t *obj_rget_thread (Exp *this) {
  return exp_rget_object(thread, this);
}

int obj_is_thread (Exp *this) {
  return exp_is_object(thread, this);
}

Exp *obj_tcp_server (TcpServer *value) {
  return exp_object(tcp_server, value);
}

TcpServer *obj_rget_tcp_server (Exp *this) {
  return exp_rget_object(tcp_server, this);
}

int obj_is_tcp_server (Exp *this) {
  return exp_is_object(tcp_server, this);
}

Exp *obj_tcp_conn (TcpConn *value) {
  return exp_object(tcp_conn, value);
}

TcpConn *obj_rget_tcp_conn (Exp *this) {
  return exp_rget_object(tcp_conn, this);
}

int obj_is_tcp_conn (Exp *this) {
  return exp_is_object(tcp_conn, this);
}
