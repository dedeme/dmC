
// regex -----------------------------

// s -> s
Val __regex_matches (char *pos, Val s, Val rg);

// s -> <bytes>
Val __regex_replace (char *pos, Val s, Val rg, Val repl);

// s -> s
Val __regex_replacef (char *pos, Val s, Val rg, Val frpl);

