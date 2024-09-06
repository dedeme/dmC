typedef struct file_LckFile {
  struct flock *lock;
  FILE *f;
} LckFile;
static Obj *file_aopen (char *path) {
  FILE *file = fopen(path, "a");
  if (!file)
    texc_throw("file.aopen",
      str_f("Fail opening '%s': %s", path, strerror(errno))
    );
  Obj *r = file_lck_new(file);
  LckFile *lf = r->value;
  lf->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, lf->lock);
  return r;
}
static char *file_base (char *path) {
  path = file_normalize(path);
  int ix = str_last_cindex(path, '/');
  if (ix != -1) return str_right(path, ix + 1);
  return path;
}
static char *file_canonical (char *path) {
  char *tmp = realpath(path, NULL);
  if (!tmp)
    texc_throw("file.canonical", str_f("'%s' can not be canonicalized", path));
  char *r = str_new(tmp);
  free(tmp);
  return r;
}
static char *file_cat (Arrs *parts) {
  if (tarr_sizeS(parts) == 0) tarr_pushS(parts, "./");
  else if (!**tarr_beginS(parts)) *tarr_beginS(parts) = "."; // First entry == ""
  return file_clean(tarr_join(parts, "/"));
}
static void file_cd (char *path) {
  if (chdir(path)) {
    texc_throw("file.cd", str_f(
      "Fail changing the working directory to %s: %s",
      path, strerror(errno)
    ));
  }
}
static char *file_clean (char *path) {
  char *s = str_trim(path);
  if (!*s) return "";
  Buf *bf = buf_new();
  int isSlash = 0;
  while (*s) {
    char ch = *s++;
    if (ch == '/') {
      if (isSlash) continue;
      isSlash = 1;
      buf_cadd(bf, ch);
      continue;
    }
    isSlash = 0;
    buf_cadd(bf, ch);
  }
  s = buf_str(bf);
  if (!s[1]) return s;
  if (str_ends(s, "/")) s = str_left(s, -1);
  Arrs *new = tarr_new_s();
  Arrs *ps = str_csplit(s, '/');
  char **p = tarr_beginS(ps);
  char **end = tarr_endS(ps);
  while (p < end) {
    char *part = *p++;
    if (!strcmp(part, ".")) continue;
    if (!strcmp(part, "..")) {
      if (tarr_sizeS(new)) tarr_popS(new);
      else texc_throw("file.clean", str_f("Bad path for cleaning '%s'", path));
      continue;
    }
    tarr_pushS(new, part);
  }
  s = tarr_join(new, "/");
  if (!*s && *path == '/')
    texc_throw("file.clean", str_f("Bad path for cleaning '%s'", path));
  return s;
}
static void file_close (Obj *file) {
  texc_check_type("file.close", file, "<file>");
  LckFile *lck = file->value;
  FILE *fl = lck->f;
  struct flock *lock = lck->lock;
  lock->l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lock);
  fclose(fl);
}
static void file_copy (char *source, char *target) {
  void fcopy (char *source , char *file) {
  if (!strcmp(source, target)) return;
    FILE *f1;
    FILE *f2;
    char  buffer[32678];
    size_t  n;
    if ((f1 = fopen(source, "rb")) == 0)
      texc_throw("file.copy",
        str_f("Fail openning '%s': %s", source, strerror(errno))
      );
    if ((f2 = fopen(target, "wb")) == 0)
      texc_throw("file.copy",
        str_f("Fail openning '%s': %s", target, strerror(errno))
      );
    while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0) {
      if (fwrite(buffer, sizeof(char), n, f2) != n)
        texc_throw("file.copy",
          str_f("Fail writing '%s': %s", target, strerror(errno))
        );
    }
    fclose(f1);
    fclose(f2);
  }
  if (file_is_directory(source)) {
    if (!file_is_directory(target))
      texc_throw("file.copy", str_f("'%s' is not a directory", target));
    char *tdir = str_f("%s/%s", target, file_base(source));
    file_del(tdir);
    file_mkdir(tdir);
    Arrs *dir = file_dir(source);
    char **p = tarr_beginS(dir);
    char **end = tarr_endS(dir);
    while (p < end) file_copy(str_f("%s/%s", source, *p++), tdir);
    return;
  }
  if (file_is_directory(target))
    target = str_f("%s/%s", target, file_base(source));
  fcopy(source, target);
}
static void file_del (char *path) {
  struct stat buf;
  if (stat(path, &buf) && errno != ENOENT)
      texc_throw("file.del",
        str_f("Fail deleting %s: %s", path, strerror(errno))
      );
  if (S_ISDIR(buf.st_mode)) {
    Arrs *names = file_dir(path);
    char **p = tarr_beginS(names);
    char **end = tarr_endS(names);
    while (p < end)
      file_del(str_f("%s/%s", path, *p++));

    if (rmdir(path) && errno != ENOENT)
      texc_throw("file.del",
        str_f("Fail deleting %s: %s", path, strerror(errno))
      );
  } else {
    if (unlink(path) && errno != ENOENT)
      texc_throw("file.del",
        str_f("Fail deleting %s: %s", path, strerror(errno))
      );
  }
}
static Arrs *file_dir (char *path) {
  DIR *d = opendir(path);
  if (!d)
    texc_throw("file.dir",
      str_f("Fail reading directory %s: %s", path, strerror(errno))
    );
  Arrs *a = tarr_new_s();
  struct dirent *res;
  while ((res = readdir(d))) {
    char *name = res->d_name;
    if (!strcmp(name, ".") || !strcmp(name, "..")) {
      continue;
    }
    tarr_pushS(a, str_new(name));
  }
  closedir(d);
  return a;
}
static int file_exists (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return FALSE;
    }
    texc_throw("file.exists",
      str_f("Fail reading %s: %s", path, strerror(errno))
    );
  }
  return TRUE;
}
static char *file_extension (char *path) {
  path = file_base(path);
  int ix = str_last_cindex(path, '.');
  if (ix == -1) return "";
  return str_right(path, ix);
}
int file_is_directory (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return FALSE;
    }
    texc_throw("file.isDirectory",
      str_f("Fail reading %s: %s", path, strerror(errno))
    );
  }
  if (S_ISDIR(buf.st_mode))
    return TRUE;
  return FALSE;
}
int file_is_link (char *path) {
  struct stat buf;
  if (lstat(path, &buf)) {
    if (errno == ENOENT) {
      return FALSE;
    }
    texc_throw("file.isLink",
      str_f("Fail reading %s: %s", path, strerror(errno))
    );
  }
  if (S_ISLNK(buf.st_mode))
    return TRUE;
  return FALSE;
}
int file_is_regular (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return FALSE;
    }
    texc_throw("file.isRegular",
      str_f("Fail reading %s: %s", path, strerror(errno))
    );
  }
  if (S_ISREG(buf.st_mode))
    return TRUE;
  return FALSE;
}
static Obj *file_lck_new(FILE *file) {
  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;
  LckFile *lf = MALLOC(LckFile);
  lf->lock = lck;
  lf->f = file;
  return obj_new("<file>", lf);
}
static void file_mkdir (char *path) {
  if (!*path || (*path == '/' && !path[1])) return;
  char *p;
  if (*path != '/') {
    char *cwd = file_wd();
    p = str_f("%s/%s", cwd, path);
  } else {
    p = path;
  }
  p = file_parent(p);
  file_mkdir(p);
  if (mkdir(path, 0755) && errno != EEXIST)
    texc_throw("file.mkdir",
      str_f("Fail making directory %s: %s" , path, strerror(errno))
    );
}
static void file_mklink (char *path, char *link) {
  if (symlink(path, link) == -1)
    texc_throw("file.mkdir",
      str_f("Fail linking '%s' to '%s: %s", link, path, strerror(errno))
    );
}
static char *file_normalize(char *path) {
  char *r = str_trim(path);
  while (str_ends(r, "/")) r = str_left(r, -1);
  return r;
}
static char *file_parent (char *path) {
  char *s = file_normalize(path);
  if (!*s)
    texc_throw("file.parent",
      str_f("'%s' has not parent directory", path)
    );
  int ix = str_last_cindex(s, '/');
  if (ix == -1) return "";
  if (ix == 0) return "/";
  return str_left(s, ix);
}
static char *file_read(char *path) {
  FILE *fl;
  size_t len = 0;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };
  fl = fopen(path, "r");
  if (!fl)
    texc_throw("file.read",
      str_f("Fail openning %s: %s", path, strerror(errno))
    );
  lck.l_type = F_RDLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);
  Buf *bf = buf_new();
  char *line = NULL;
  while (getline(&line, &len, fl) != -1) {
    buf_add(bf, line);
    free(line); // free ok
    line = NULL;
  }
  free(line); // free ok
  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);
  fclose(fl);
  return str_new(buf_str(bf));
}
static Obj *file_read_bin (Obj *file, int64_t buf) {
  texc_check_type("file.readBin", file, "<file>");
  LckFile *lf = file->value;
  unsigned char bs[buf];
  int len = (int)fread(bs, 1, buf, lf->f);
  if (len == -1) {
    file_close(file);
    texc_throw("file.readBin",
      str_f("Fail file_read_bin_buf: %s", strerror(errno))
    );
  }
  if (len == 0) {
    return bytes_new(0);
  }
  return bytes_from_bytes(bs, len);
}
static Arrs *file_read_line (Obj *file) {
  texc_check_type("file.readBin", file, "<file>");
  LckFile *lf = file->value;
  size_t len = 0;
  errno = 0;
  char *line= NULL;
  if (getline(&line, &len, lf->f) != -1) {
    char *r = str_new(line);
    free(line); // free ok
    while (str_ends(r, "\n") || str_ends(r, "\r")) r = str_left(r, -1);
    return tarr_news(r, 1);
  }
  free(line); // free ok
  if (errno) {
    file_close(file);
    texc_throw("file.readLine",
      str_f("Fail file_read_line: %s", strerror(errno))
    );
  }
  return tarr_new_s();
}
static void file_rename (char *old, char *new) {
  if (rename(old, new) == -1)
    texc_throw("file.readLine",
      str_f("Fail renaming '%s' to '%s: %s", old, new, strerror(errno))
    );
}
static Obj *file_ropen (char *path) {
  FILE *file = fopen(path, "r");
  if (!file)
    texc_throw("file.ropen",
      str_f("Fail opening '%s': %s", path, strerror(errno))
    );
  Obj *r = file_lck_new(file);
  LckFile *lf = r->value;
  lf->lock->l_type = F_RDLCK;
  fcntl(fileno(file), F_SETLKW, lf->lock);
  return r;
}
struct stat *file_info (char *path) {
  struct stat *r = MALLOC(struct stat);
  if (stat(path, r))
    texc_throw("file.size | file.tm",
      str_f("Fail reading %s: %s", path, strerror(errno))
    );
  return r;
}
static int64_t file_size (char *path) {
  struct stat *i = file_info (path);
  return i->st_size;
}
static int64_t file_tm (char *path){
  struct stat *i = file_info (path);
  return ((int64_t)i->st_mtime) * 1000;
}
static char *file_tmp (char *dir, char *prefix) {
  if (!*dir) dir = "/tmp";
  char *fpath = NULL;
  for (;;) {
    char *k = cryp_genk(16);
    char *pid = str_f("%d", (int)getpid());
    char *v = str_left(cryp_encode(k, pid), 8);
    fpath = str_f("%s/%s%s", dir, prefix, str_creplace(v, '/', '-'));
    if (!file_exists(fpath)) break;
  }
  return fpath;
}
static char *file_wd (void) {
  char *d = getcwd(NULL, 0);
  if (!d)
    texc_throw("file.wd",
      str_f("Working directory can no be find: %s", strerror(errno))
    );
  char *r = str_new(d);
  free(d);  // free is ok
  return r;
}
static Obj *file_wopen (char *path) {
  FILE *file = fopen(path, "w");
  if (!file)
    texc_throw("file.wopen",
      str_f("Fail opening '%s': %s", path, strerror(errno))
    );
  Obj *r = file_lck_new(file);
  LckFile *lf = r->value;
  lf->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, lf->lock);
  return r;
}
static void file_write (char *path, char *text) {
  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };
  fl = fopen(path, "w");
  if (!fl)
    texc_throw("file.write",
      str_f("Fail openning %s: %s", path, strerror(errno))
    );
  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);
  error = fputs(text, fl);
  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);
  if (error == EOF || error < 0) {
    fclose(fl);
    texc_throw("file.write",
      str_f("Fail writing '%s': %s", path, strerror(error))
    );
  }
  fclose(fl);
}
static void file_write_bin (Obj *file, Obj *data) {
  texc_check_type("file.writeBin", file, "<file>");
  texc_check_type("file.writeBin", data, "<bytes>");
  LckFile *lf = file->value;
  Bytes *bs = data->value;
  if (fwrite(bs->bs, bs->length, 1, lf->f) == -1)
    texc_throw("file.writeBin",
      str_f("Fail file_write_bin: %s", strerror(errno))
    );
}
static void file_write_text (Obj *file, char *text) {
  texc_check_type("file.writeText", file, "<file>");
  LckFile *lf = file->value;
  int error = fputs(text, lf->f);
  if (error == EOF || error < 0) {
    file_close(file);
    texc_throw("file.writeText",
      str_f("Fail file_write_line: %s", strerror(errno))
    );
  }
}
