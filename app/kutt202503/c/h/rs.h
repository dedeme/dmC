
// rs ----------------------------------

// * -> [*]
Val __rs_some (Val v);
// * -> [*,s]
Val __rs_ok (Val v);
// s -> [*,s]
Val __rs_fail (Val v);
// [*, s] -> *
Val __rs_get (Val rs);
// [*, s] -> s
Val __rs_error (Val rs);

