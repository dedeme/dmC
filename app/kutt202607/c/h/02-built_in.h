// built-in -----------------------------

// Jump to go out of try with the following 'reference':
//    2: break. 3: continue. 4: return.
void ___built_jump (int reference);
// Returns path of module - line kutt.
char *___built_mpos(char *module_ln);
// Adapts la c stack line to a ktt stack one.
char *___built_stack_line (char *e);

// s,i,i -> i
Val ___built_divi(char *pos, Val n1, Val n2);
// s,f,f -> f
Val ___built_divf(char *pos, Val n1, Val n2);
// s,*,*->b
Val ___built_eq (Val type, Val value1, Val value2);
// s,*->s
Val ___built_to_str_trace (Val type, Val value);
// s,*->s
Val ___built_to_str (Val type, Val value);
// s, s -> ()
void ___built_throw (char *pos, Val msg);
// b,s,s,*->()
void ___built_trace (Val isComplete, char *pos, Val type, Val value);

