// js ----------------------------------

// s->b
Val __js_isNull (Val json);
// s->[.s]
Val __js_ra (Val json);
// s->b
Val __js_rb (Val json);
// s->f
Val __js_rf (Val json);
// s->i
Val __js_ri (Val json);
// s->{s}
Val __js_ro (Val json);
// s->s
Val __js_rs (Val json);
// [.s]->s
Val __js_wa(Val a);
// b->s
Val __js_wb(Val value);
// f->s
Val __js_wf(Val n);
// f->s
Val __js_wf2(Val n, Val dec);
// i->s
Val __js_wi(Val n);
// ->s
Val __js_wn(void);
// {s}->s
Val __js_wo(Val m);
// s->s
Val __js_ws(Val s);

