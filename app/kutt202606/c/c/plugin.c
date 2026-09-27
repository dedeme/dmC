
// plugin -------------------------------

// <plugin> -> ()
void __plugin_close (Val pl) {
  dlclose(pl.o);
}

// <plugin>, s, [*] -> *
Val __plugin_exec (char *pos, Val rt, Val pl, Val fn, Val vs) {
  void *handle = pl.o;
  char *fns = fn.s;
  Val (*bfn)(Val) = (Val(*)(Val)) dlsym(handle, fns);
  if (!bfn)
    ___built_throw(pos, (Val)(str_f("%s", dlerror())));
  return bfn(vs);
}

// s -> <plugin>
Val __plugin_open (char *pos, Val path) {
  void *handle = dlopen(path.s, RTLD_NOW);
  if (!handle)
    ___built_throw(pos, (Val)(str_f("%s", dlerror())));
  return (Val)(void *)handle;
}
