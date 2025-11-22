
// sys ---------------------------------

// \s -> ()
void __sys_init(int argc, char **argv, char *compiler_root);

// -----------------

// () -> [s]
Val __sys_args (void);

// s, [s] -> [. [s]s]
Val __sys_cmd (Val command, Val args);

// s, [s] -> [. ss]
Val __sys_cmd2 (Val command, Val args);

// () -> s
Val __sys_compilerRoot (void);

// () -> {s}
Val __sys_environ (void);

// i -> ()
void __sys_exit (Val code);

// () -> s
Val __sys_getLocale (void);

// () -> s
Val __sys_home (void);

// () -> s
Val __sys_mainPath (void);

// () -> i
Val __sys_pid (void);

// s -> ()
void __sys_printError (Val text);

// () -> s
Val __sys_readLine (void);

// s -> ()
void __sys_setLocale (Val charset);

// i -> ()
void __sys_sleep (Val millis);

// () -> s
Val __sys_user (void);

