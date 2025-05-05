
// tcp ---------------------------------

// <tcpServer> -> [. [<tcpConn>]s]
Val __tcp_accept (Val server) {
  Rs *rs = tcp_accept(server.o);
  void *conn = rs_get(rs);
  if (!conn) return __rs_fail((Val)rs_error(rs));
  return __rs_ok((Val)conn);
}

// <tcpConn> -> ()
void __tcp_closeConnection (Val conn) {
  tcp_close_conn(conn.o);
}

// <tcpServer> -> ()
void __tcp_closeServer (Val server) {
  tcp_close_server(server.o);
}

// s -> [. [<tcpConn>]s]
Val __tcp_dial (Val server_name) {
  char *sv = server_name.s;
  int ok = TRUE;
  Arr *ps = str_csplit(sv, ':');
  if (arr_size(ps) != 2) ok = FALSE;
  if (ok && !math_digits(arr_get(ps, 1))) ok = FALSE;
  if (!ok)
    return __rs_fail((Val)str_f("Bad sever:port (%s)", sv));

  Rs *rs = tcp_dial(arr_get(ps, 0), math_stoi(arr_get(ps, 1)));
  void *conn = rs_get(rs);
  if (!conn) return __rs_fail((Val)rs_error(rs));
  return __rs_ok((Val)conn);
}

// <tcpConn>, i , i -> [. [<bytes>], b]
Val __tcp_read (Val conn, Val lim, Val seconds) {
  Rs *rs = tcp_read(conn.o, lim.i, seconds.i);
  void *bs = rs_get(rs);
  if (!bs) return __rs_fail((Val)rs_error(rs));
  return __rs_ok((Val)bs);
}

// i, i -> <tcpServer>
Val __tcp_server (Val port, Val nconns) {
  return (Val)(void *)tcp_server(port.i, nconns.i);
}

// <tcpConn>, <bytes> -> s
Val __tcp_write (Val conn, Val data) {
  return (Val)tcp_write(conn.o, data.o);
}
