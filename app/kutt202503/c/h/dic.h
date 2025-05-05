// dic ---------------------------------

// {A} -> {A}
Val __dic_copy (Val d);
// [[.sA]] -> {A}
Val __dic_fromArr (Val a);
// [[.sA]] -> {A}
Val __dic_fromIter (Val it);
// s,(s->A) -> {A}
Val __dic_fromJs (Val s, Val fn);
// Auxiliar of __js_ro
// Map<char>->{s}
Val __dic_from_smap (Map *m);
// {A},s -> [A]
Val __dic_get(Val d, Val key);
// s,{A},s -> A
Val __dic_get2(char *pos, Val d, Val key);
// {A},s -> b
Val __dic_hasKey(Val d, Val key);
// {A}->[s]
Val __dic_keys (Val d);
// {A}A->B -> {B}
Val __dic_map (Val d, Val fn);
// i,*[s,A]->{A}
Val __dic_new_from_carr (Val n, Val *els);
// {A},s,A->()
void __dic_put(Val d, Val key, Val value);
// {A},s -> ()
void __dic_remove(Val d, Val key);
// s,{A},s, A -> ()
void __dic_set(char *pos, Val d, Val key, Val value);
// {A} -> i
Val __dic_size(Val d);
// {A}->[[. sA]]
Val __dic_toArr (Val d);
// {A}->[[. sA]]
Val __dic_toIter (Val d);
// {A},(A->s)->s
Val __dic_toJs (Val d, Val fn);
// Auxiliar of __js_wo
// {A}->Map<char>
Map *__dic_to_smap (Val a);
// {A}->[A]
Val __dic_values (Val d);

