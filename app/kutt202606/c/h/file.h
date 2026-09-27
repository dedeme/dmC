
// file --------------------------------

// s -> <file>
Val __file_aopen (Val path);

// s -> s
Val __file_base (Val path);

// s -> s
Val __file_canonical (char *pos, Val path);

// s, [s] -> s
Val __file_cat (Val path, Val rpaths);

//  d -> ()
void __file_cd (Val path);

// s -> s
Val __file_clean (Val path);

// <file> -> ()
void __file_close (Val file);

// s, s -> ()
void __file_copy (char *pos, Val source, Val target);

// s -> ()
void __file_del (Val path);

// s -> [s]
Val __file_dir (Val path);

// s -> b
Val __file_exists (Val path);

// s -> s
Val __file_extension (Val path);

// s -> b
Val __file_isDirectory (Val path);

// s -> b
Val __file_isLink (Val path);

// s -> b
Val __file_isRegular (Val path);

// s -> ()
void __file_mkdir (Val path);

// s, s -> ()
void __file_mklink (Val path, Val link);

// s -> s
Val __file_parent (Val path);

// s -> s
Val __file_read (Val path);

// <file>, i -> <bytes>
Val __file_readBin (Val file, Val buf);

// <file> -> [s]
Val __file_readLine (Val file);

// s, s -> ()
void __file_rename (Val old_path, Val new_path);

// s -> <file>
Val __file_ropen (Val path);

// s -> i
Val __file_size (Val path);

// s -> i
Val __file_tm (Val path);

// s, s -> s
Val __file_tmp (Val dir, Val name);

//  -> s
Val __file_wd (void);

// s -> <file>
Val __file_wopen (Val path);

// s, s -> ()
void __file_write (Val path, Val tx);

// <file>, <bytes> -> ()
void __file_writeBin (Val file, Val bs);

// <file>, s -> ()
void __file_writeText (Val file, Val tx);

