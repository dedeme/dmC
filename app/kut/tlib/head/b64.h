static char *b64_table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char *b64_decode(char *b64);
static Obj *b64_decode_bytes(char *b64);
static char *b64_encode(char *s);
static char *b64_encode_bytes(Obj *bs);
