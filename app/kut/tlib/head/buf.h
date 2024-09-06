typedef struct buf_Buf Buf;

static Buf *buf_new ();
static Buf *buf_new_bf (int buffer_size);
static char *buf_str (Buf *this);
static int buf_len (Buf *this);
static void buf_add_buf (Buf *this, char *data, int length);
static void buf_add(Buf *this, char *data);
static void buf_cadd(Buf *this, char data);
static void buf_reset(Buf *this);
