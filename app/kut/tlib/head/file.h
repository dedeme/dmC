static Obj *file_aopen (char *path);
static char *file_base (char *path);
static char *file_canonical (char *path);
static char *file_cat (Arrs *parts);
static void file_cd (char *path);
static char *file_clean (char *path);
static void file_close (Obj *file);
static void file_copy (char *source, char *target);
static void file_del (char *path);
static Arrs *file_dir (char *path);
static int file_exists (char *path);
static char *file_extension (char *path);
struct stat *file_info (char *path);
static int file_is_directory (char *path);
static int file_is_link (char *path);
static int file_is_regular (char *path);
static Obj *file_lck_new(FILE *file);
static void file_mkdir (char *path);
static void file_mklink (char *path, char *link);
static char *file_normalize(char *path);
static char *file_parent (char *path);
static char *file_read (char *path);
static Obj *file_read_bin (Obj *file, int64_t buf);
static Arrs *file_read_line (Obj *file);
static void file_rename (char *old, char *new);
static Obj *file_ropen (char *path);
static int64_t file_size (char *path);
static int64_t file_tm (char *path);
static char *file_tmp (char *dir, char *prefix);
static char *file_wd (void);
static Obj *file_wopen (char *path);
static void file_write (char *path, char *text);
static void file_write_bin (Obj *file, Obj *bs);
static void file_write_text (Obj *file, char *text);
