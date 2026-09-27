
// file --------------------------------

// s -> <file>
Val __file_aopen (Val path) {
  return (Val)(void *)file_aopen(path.s);
}

// s -> s
Val __file_base (Val path) {
  return (Val)path_base (path.s);
}

// s -> s
Val __file_canonical (char *pos, Val path) {
  char *r = opt_get(path_canonical(path.s));
  if (!r)
    ___built_throw(pos, (Val)(str_f("'%s' can not be canonicalized", path.s)));
  return (Val)r;
}

// s, [s] -> s
Val __file_cat (Val path, Val rpaths) {
  __arr_unshift(rpaths, *(path.s) ? path : (Val)".");
  return __file_clean(__arr_join(rpaths, (Val)"/"));
}

//  d -> ()
void __file_cd (Val path) {
  file_cd(path.s);
}

// s -> s
Val __file_clean (Val path) {
  return (Val)path_clean(path.s);
}

// <file> -> ()
void __file_close (Val file) {
  file_close(file.o);
}

// s, s -> ()
void __file_copy (char *pos, Val source, Val target) {
  void copy (char *source, char *target) {
    if (file_is_directory(source)) {
      if (file_exists(target) && !file_is_directory(target))
        ___built_throw(pos, (Val)str_f("'%s' is not a directory", target));

      char *tdir = path_cat(target, path_base(source), NULL);
      file_del(tdir);
      file_mkdir(tdir);
      EACH(file_dir(source), char, fname) {
        copy(path_cat(source, fname, NULL), tdir);
      }_EACH
      return;
    }
    if (file_is_directory(target))
      target = path_cat(target, path_base(source), NULL);

    file_copy(source, target);
  }
  return copy(source.s, target.s);
}

// s -> ()
void __file_del (Val path) {
  file_del(path.s);
}

// s -> [s]
Val __file_dir (Val path) {
  Val r = __arr_new();
  EACH(file_dir(path.s), char, name) {
    __arr_push(r, (Val)name);
  }_EACH
  return r;
}

// s -> b
Val __file_exists (Val path) {
  return (Val)file_exists(path.s);
}

// s -> s
Val __file_extension (Val path) {
  return (Val)path_extension(path.s);
}

// s -> b
Val __file_isDirectory (Val path) {
  return (Val)file_is_directory(path.s);
}

// s -> b
Val __file_isLink (Val path) {
  return (Val)file_is_link(path.s);
}

// s -> b
Val __file_isRegular (Val path) {
  return (Val)file_is_regular(path.s);
}

// s -> ()
void __file_mkdir (Val path) {
  file_mkdir(path.s);
}

// s, s -> ()
void __file_mklink (Val path, Val link) {
  file_link(path.s, link.s);
}

// s -> s
Val __file_parent (Val path) {
  return (Val)path_parent(path.s);
}

// s -> s
Val __file_read (Val path) {
  return (Val)file_read(path.s);
}

// <file>, i -> <bytes>
Val __file_readBin (Val file, Val buf) {
  return (Val)(void *)file_read_bin_buf(file.o, buf.i);
}

// <file> -> [s]
Val __file_readLine (Val file) {
  char *l = file_read_line(file.o);
  if (*l) {
    if (l[strlen(l) - 1] == '\n') l = str_left(l, -1);
    return __rs_some((Val)l);
  }
  return __arr_new();
}

// s, s -> ()
void __file_rename (Val old_path, Val new_path) {
  file_rename(old_path.s, new_path.s);
}

// s -> <file>
Val __file_ropen (Val path) {
  return (Val)(void *)file_ropen(path.s);
}

// s -> i
Val __file_size (Val path) {
  return (Val)(int64_t)file_size(path.s);
}

// s -> i
Val __file_tm (Val path) {
  return (Val)file_modified(path.s);
}

// s, s -> s
Val __file_tmp (Val dir, Val name) {
  return (Val)file_tmp(dir.s, name.s);
}

//  -> s
Val __file_wd (void) {
  return (Val)file_wd();
}

// s -> <file>
Val __file_wopen (Val path) {
  return (Val)(void *)file_wopen(path.s);
}

// s, s -> ()
void __file_write (Val path, Val tx) {
  file_write(path.s, tx.s);
}

// <file>, <bytes> -> ()
void __file_writeBin (Val file, Val bs) {
  file_write_bin(file.o, bs.o);
}

// <file>, s -> ()
void __file_writeText (Val file, Val tx) {
  file_write_text(file.o, tx.s);
}

