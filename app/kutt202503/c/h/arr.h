// Varr --------------------------------

// [.A], (A->b) -> b
Val __arr_all (Val a, Val fn);
// [.A], (A->b) -> b
Val __arr_any (Val a, Val fn);
// [.A],[.A]->()
void __arr_cat (Val a, Val a2);
// [.A],[.A]->[.A]
Val __arr_cat2 (Val a, Val a2);
// [.A]->()
void __arr_clear (Val a);
// [.A]->[.A]
Val __arr_copy (Val a);
// [.A],i->[.A]
Val __arr_drop (Val a, Val n);
// [.A], (A->b) -> b
Val __arr_dropWhile (Val a, Val fn);
// [.A], (A,A->b) -> b
Val __arr_duplicates (Val a, Val fn);
// [.A], (A->()) -> ()
void __arr_each (Val a, Val fn);
// [.A], (A,i->()) -> ()
void __arr_eachIx (Val a, Val fn);
// [.A], (A->b) -> b
Val __arr_filter (Val a, Val fn);
// [.A], (A->b) -> ()
void __arr_filterIn (Val a, Val fn);
// [.A], (A->b) -> ([.A]|[])
Val __arr_find (Val a, Val fn);
// s,[! A] -> [A]
Val __arr_fromIter (Val it);
// s,(s->A) -> [A]
Val __arr_fromJs (Val s, Val fn);
// Auxiliar of __js_ra
// Arr<char>->[.s]
Val __arr_from_sarr (Arr *a);
// [.A],i->A
Val __arr_get (Val a, Val ix);
// s,[.A],i->A
Val __arr_get2 (char *pos, Val a, Val ix);
// [.A], (A->b) -> i
Val __arr_index (Val a, Val fn);
// s,[.A],i,A->()
void __arr_insert (char *pos, Val a, Val ix, Val e);
// s,[.A],i,[.A]->()
void __arr_insertArr (char *pos, Val a, Val ix, Val other);
// s,[.A],i,[.A],i,i->()
void __arr_insertRange (char *pos, Val a, Val ix, Val other, Val begin, Val end);
// [.s].s->s
Val __arr_join(Val a, Val sep);
// [.A],(A->B)->[.B]
Val __arr_map(Val a, Val fn);
// ->[A.]
Val __arr_new (void);
// A,i->[.A]
Val __arr_new_fill (Val el, Val n);
// i,*A->[.A]
Val __arr_new_from_carr (Val n, Val *els);
// [.A]->A
Val __arr_peek (char *pos, Val a);
// [.A]->A
Val __arr_pop (char *pos, Val a);
// [.A],A->()
void __arr_push (Val a, Val el);
// [.A]->A
Val __arr_reduce (Val a, Val seed, Val fn);
// [.A],i->[.A]
Val __arr_remove (char *pos, Val a, Val ix);
// [.A],i,i->[.A]
Val __arr_removeRange (char *pos, Val a, Val begin, Val end);
// [.A]->[.A]
Val __arr_reverse (Val a);
// [.A]->[.A]
void __arr_reverseIn (Val a);
// [.A],i,A->()
void __arr_set (Val a, Val ix, Val el);
// s,[.A],i,A->()
void __arr_set2 (char *pos, Val a, Val ix, Val el);
// s,[.A],i,[.A]->()
void __arr_setArr (char *pos, Val a, Val ix, Val other);
// s,[.A],i,[.A],i,i->()
void __arr_setRange (char *pos, Val a, Val ix, Val other, Val begin, Val end);
// [.A]->A
Val __arr_shift (char *pos, Val a);
// [.A]->A
void __arr_shuffle (Val a);
// [.A]->i
Val __arr_size (Val a);
// [.A],(AA->b)->()
void __arr_sort (Val a, Val fn);
// [.A],i,i->[.A]
Val __arr_sub (Val a, Val begin, Val end);
// [.A],i->[.A]
Val __arr_sub0 (Val a, Val begin);
// [.A],i->[A.]
Val __arr_take (Val a, Val n);
// [.A], (A->b) -> b
Val __arr_takeWhile (Val a, Val fn);
// [.A] -> [!A]
Val __arr_toIter (Val a);
// [A],(A->s) -> s
Val __arr_toJs (Val a, Val fn);
// Auxiliar of __js_wa
// [.s]->Arr<char>
Arr *__arr_to_sarr (Val a);
// [.A],A->()
void __arr_unshift (Val a, Val e);
// [.[AB]] -> [[.A][.B]]
Val __arr_unzip (Val a);
// [.[ABC]] -> [[.A][.B][.C]]
Val __arr_unzip3 (Val a);
// [.A],[.B] -> [.[AB]]
Val __arr_zip (Val a1, Val a2);
// [.A],[.B][.C] -> [.[ABC]]
Val __arr_zip3 (Val a1, Val a2, Val a3);
