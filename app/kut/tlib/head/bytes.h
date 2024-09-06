typedef struct bytes_Bytes {
  unsigned char *bs;
  int64_t length;
} Bytes;
static Obj *bytes_add (Obj *this, Obj *other);
static Obj *bytes_drop (Obj *this, int64_t n);
static Obj *bytes_from_arr (Arri *a);
static Obj *bytes_from_bytes(unsigned char *bs, int length);
static Obj *bytes_from_str (char *s);
static int64_t bytes_get (Obj *this, int64_t ix);
static Obj *bytes_new (int64_t length);
static void bytes_set (Obj *this, int64_t ix, int64_t v);
static int64_t bytes_size (Obj *this);
static Obj *bytes_take (Obj *this, int64_t n);
static Arri *bytes_to_arr (Obj *this);
static char *bytes_to_str (Obj *this);
