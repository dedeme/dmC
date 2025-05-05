
// js ----------------------------------

// s->b
Val __js_isNull (Val json) {
  return (Val)js_is_null(json.s);
}
// s->[.s]
Val __js_ra (Val json) {
  return __arr_from_sarr(js_ra(json.s));
}
// s->b
Val __js_rb (Val json) {
  return (Val)js_rb(json.s);
}
// s->f
Val __js_rf (Val json) {
  return (Val)js_rd(json.s);
}
// s->i
Val __js_ri (Val json) {
  return (Val)js_rl(json.s);
}
// s->{s}
Val __js_ro (Val json) {
  return __dic_from_smap(js_ro(json.s));
}
// s->s
Val __js_rs (Val json) {
  return (Val)js_rs(json.s);
}
// [.s]->s
Val __js_wa(Val a) {
  return (Val)js_wa(__arr_to_sarr(a));
}
// b->s
Val __js_wb(Val value) {
  return (Val)js_wb(value.b);
}
// f->s
Val __js_wf(Val n) {
  return (Val)math_ftos(n.f, 9);
}
// f->s
Val __js_wf2(Val n, Val dec) {
  return (Val)math_ftos(n.f, dec.i);
}
// i->s
Val __js_wi(Val n) {
  return (Val)math_itos(n.i);
}
// ->s
Val __js_wn(void) {
  return (Val)js_wn();
}
// {s}->s
Val __js_wo(Val m) {
  return (Val)js_wo(__dic_to_smap(m));
}
// ->s
Val __js_ws(Val s) {
  return (Val)js_ws(s.s);
}

