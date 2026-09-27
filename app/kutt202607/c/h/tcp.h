
// tcp -------------------------------

// <tcpServer> -> [. [<tcpConn>]s]
Val __tcp_accept (Val server);

// <tcpConn> -> ()
void __tcp_closeConnection (Val conn);

// <tcpServer> -> ()
void __tcp_closeServer (Val server);

// s -> [. [<tcpConn>]s]
Val __tcp_dial (Val server_name);

// <tcpConn>, i , i -> [. [<bytes>], b]
Val __tcp_read (Val conn, Val lim, Val seconds);

// i, i -> <tcpServer>
Val __tcp_server (Val port, Val nconns);

// <tcpConn>, <bytes> -> s
Val __tcp_write (Val conn, Val data);
