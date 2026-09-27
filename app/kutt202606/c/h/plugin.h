
// plugin -------------------------------

// <plugin> -> ()
void __plugin_close (Val pl);

// <plugin>, s -> *
Val __plugin_exec (char *pos, Val rt, Val pl, Val fn, Val vs);

// s -> <plugin>
Val __plugin_open (char *pos, Val path);

