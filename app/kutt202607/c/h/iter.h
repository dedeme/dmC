
// iter ------------------------------

// [!A], (A->b) -> b
Val __iter_all (Val it, Val fn);

// [!A], (A->b) -> b
Val __iter_any (Val it, Val fn);

// [!A], [!A] -> [!A]
Val __iter_cat (Val it1, Val it2);

// [!A] -> i
Val __iter_count (Val it);

// [!A],i->[!A]
Val __iter_drop (Val it, Val n);

// [!A],(A->b)->[!A]
Val __iter_dropWhile (Val it, Val fn);

// [!A],(A->())->()
void __iter_each (Val it, Val fn);

// [!A],(A,i->())->()
void __iter_eachIx (Val it, Val fn);

// -> [!A]
Val __iter_empty (Val fake);

// [!A],(A->b)->[!A]
Val __iter_filter (Val it, Val fn);

// [!A], (A->b) -> [A]
Val __iter_find (Val it, Val fn);

// s,(s->A) -> [!A]
Val __iter_fromJs (Val s, Val fn);

// [! A] -> b
Val __iter_hasNext (Val it);

// [!A], (A->b) -> i
Val __iter_index (Val it, Val fn);

// [!A], (A->B) -> [!B]
Val __iter_map (Val it, Val fn);

// [! A] -> A
Val __iter_next (Val it);

// [. O, (O->[E])]->[!E]
Val __iter_new (Val tuple);

// O, (O->[E]) -> [!E]
Val __iter_new_aux (Val obj, Val (*fn) (Val));

// [! A] -> A
Val __iter_peek (Val it);

// s,[i] -> [! i]
Val __iter_range (char *pos, Val vs);

// [!A], B, (BA->B) -> B
Val __iter_reduce (Val it, Val seed, Val fn);

// [!A],i->[!A]
Val __iter_take (Val it, Val n);

// [!A],(A->b)->[!A]
Val __iter_takeWhile (Val it, Val fn);

// [!A],(A->s) -> s
Val __iter_toJs (Val it, Val fn);

// A -> [!A]
Val __iter_unary (Val value);

