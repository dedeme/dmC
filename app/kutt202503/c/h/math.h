// math --------------------------------

// f->f
Val __math_abs (Val n);
// f->f
Val __math_acos (Val n);
// f->f
Val __math_acosh (Val n);
// f->f
Val __math_asin (Val n);
// f->f
Val __math_asinh (Val n);
// f->f
Val __math_atan (Val n);
// f->f
Val __math_atanh (Val n);
// f->f
Val __math_ceil (Val n);
// f->f
Val __math_cos (Val n);
// f->f
Val __math_cosh (Val n);
// ()->f
Val __math_e (void);
// fff->b
Val __math_eq (Val n1, Val n2, Val gap);
// f->f
Val __math_exp (Val n);
// f->f
Val __math_exp2 (Val n);
// f->f
Val __math_floor (Val n);
// s->[f]|[]
Val __math_fromEn (Val s);
// s->[i]|[]
Val __math_fromHex (Val s);
// s->[f]|[]
Val __math_fromIso (Val s);
// f->i
Val __math_ftoi (Val n);
// f->s
Val __math_ftos (Val n);
// fi->s
Val __math_ftos2 (Val n, Val dec);
// s->b
Val __math_isDigits (Val n);
// i->f
Val __math_itof (Val n);
// i->s
Val __math_itoh (Val n);
// i->s
Val __math_itos (Val n);
// f->f
Val __math_log (Val n);
// f->f
Val __math_log10 (Val n);
// f->f
Val __math_log2 (Val n);
// ff->f
Val __math_max (Val n1, Val n2);
// ()->f
Val __math_maxFloat (void);
// ()->i
Val __math_maxInt (void);
// ff->f
Val __math_min (Val n1, Val n2);
// ()->i
Val __math_minInt (void);
// ()->f
Val __math_pi (void);
// ff->f
Val __math_pow (Val base, Val exp);
// i->f
Val __math_pow10 (Val exp);
// ()->f
Val __math_rnd (void);
// i->i
Val __math_rndi (Val top);
// fi->f
Val __math_round (Val n, Val dec);
// f->f
Val __math_sin (Val n);
// f->f
Val __math_sinh (Val n);
// f->f
Val __math_sqrt (Val n);
// s->[f]|[]
Val __math_stof (Val s);
// s->[i]|[]
Val __math_stoi (Val s);
// f->f
Val __math_tan (Val n);
// f->f
Val __math_tanh (Val n);
// f->f
Val __math_trunc (Val n);
