
// bytes -----------------------------

// <bytes>,<bytes> -> <bytes>
Val __bytes_add (Val bs1, Val bs2);
// <bytes>,i -> <bytes>
Val __bytes_drop (Val bs, Val begin);
// [.i] -> <bytes>
Val __bytes_fromArr (Val a);
// s -> <bytes>
Val __bytes_fromStr (Val str);
// s,<bytes>,i->i
Val __bytes_get (char *pos, Val bs, Val ix);
// i -> <bytes>
Val __bytes_new (Val size);
// s,<bytes>,i,i->()
void __bytes_set (char *pos, Val bs, Val ix, Val byte);
// <bytes> -> i
Val __bytes_size (Val bs);
// <bytes>,i -> <bytes>
Val __bytes_take (Val bs, Val end);
// <bytes> -> [.i]
Val __bytes_toArr (Val bs);
// <bytes> -> s
Val __bytes_toStr (Val bs);
