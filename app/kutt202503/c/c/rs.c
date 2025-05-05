
// * -> [*]
Val __rs_some (Val v) {
  return __arr_new_from_carr((Val) 1, (Val[]){v});
}

// * -> [[*],s]
Val __rs_ok (Val v) {
  return __arr_new_from_carr((Val) 2, (Val[]){__rs_some(v), (Val)""});
}

// s -> [*,s]
Val __rs_fail (Val v) {
  return __arr_new_from_carr((Val) 2, (Val[]){__arr_new(), v});
}

// [[*], s] -> *
Val __rs_get (Val rs) {
  return *((*rs.a->begin).a->begin);
}

// [[*], s] -> s
Val __rs_error (Val rs) {
  return rs.a->begin[1];
}

