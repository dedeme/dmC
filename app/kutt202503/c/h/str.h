
// str ---------------------------------

// s,s -> b
Val __str_cmp (Val s1, Val s2);
// s,s -> b
Val __str_ends (Val str, Val substr);
// s,[s.] -> s
Val __str_fmt (char *pos, Val format, Val args);
// s -> s
Val __str_fromIso (Val s);
// [.s] -> s
Val __str_fromRunes (Val a);
// [.i] -> s
Val __str_fromUtf16 (Val codepoints);
// s,s,i -> s
Val __str_get(char *pos, Val s, Val ix);
// s,s -> i
Val __str_index (Val str, Val substr);
// s,s,i -> i
Val __str_indexFrom (Val str, Val substr, Val start);
// s,s -> i
Val __str_lastIndex (Val str, Val substr);
// s,i -> [.s]
Val __str_left (Val str, Val end);
// s,i -> s
Val __str_left_slice(Val str, Val begin);
// s -> i
Val __str_len (Val str);
// s -> s
Val __str_ltrim (Val str);
// s,s,s -> s
Val __str_replace (Val s, Val old, Val new);
// s,i -> [.s]
Val __str_right (Val str, Val end);
// s,i -> s
Val __str_right_slice(Val str, Val begin);
// s -> s
Val __str_rtrim (Val str);
// s,s -> [s.]
Val __str_split (Val str, Val sep);
// s,s -> [s.]
Val __str_splitTrim (Val str, Val sep);
// s,s -> b
Val __str_starts (Val str, Val substr);
// s,i,i -> [.s]
Val __str_sub(Val str, Val begin, Val end);
// s,i,i -> s
Val __str_sub_slice(Val str, Val begin, Val end);
// s -> s
Val __str_toLower (Val str);
// s -> [.s]
Val __str_toRunes (Val str);
// s -> s
Val __str_toUpper (Val str);
// s -> [.i]
Val __str_toUtf16 (Val str);
// s -> s
Val __str_trim (Val str);
