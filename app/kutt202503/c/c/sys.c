
// sys ---------------------------------

static Val __sys_compiler_root_v = (Val) "";
static Val __sys_mainPath_v = (Val) "";
static Val __sys_args_v = (Val) "";

static void __sys_shandler (int sig) {
  ___built_throw("built-in:0", (Val)"Arithmetic exception");
}
static void __sys_sshandler (int sig) {
  ___built_throw("built-in:0", (Val)"Segmentation violation");
}

// \s -> ()
void __sys_init(int argc, char **argv, char *compiler_root) {
  sys_init();
  signal (SIGFPE, __sys_shandler);
  signal (SIGSEGV, __sys_sshandler);
  __sys_compiler_root_v = (Val)compiler_root;
  __sys_mainPath_v = (Val)*argv;
  __sys_args_v = __arr_new();
  for (int i = 1; i < argc; ++i) __arr_push(__sys_args_v, (Val)argv[i]);
}

// -----------------

// () -> [s]
Val __sys_args (void) {
  return __arr_copy(__sys_args_v);
}

Val __sys_cmd (Val command, Val args) {
  Val rs = __sys_cmd2(command, args);
  Val *ok = rs.a->begin;
  Val *err = ok + 1;
  Val r = __arr_new();
  if (*(err->s)) __arr_push(r, __arr_new());
  else __arr_push(r, __rs_some(*ok));
  __arr_push(r, *err);
  return r;
}

// s, [s] -> [. ss]
Val __sys_cmd2 (Val command, Val args) {
  // Arr<char>
  Arr *acmd = arr_new();
  arr_push(acmd, command.s);

  Val *p = args.a->begin;
  Val *end = args.a->end;
  while (p < end) arr_push(acmd, (*p++).s);

  // Tp<char, char>
  Tp *tp = sys_cmd2(acmd);
  Val r = __arr_new();
  __arr_push(r, (Val)tp->e1);
  __arr_push(r, (Val)tp->e2);
  return r;
}

// () -> s
Val __sys_compilerRoot (void) {
  return __sys_compiler_root_v;
}

// () -> {s}
Val __sys_environ (void) {
  return __dic_from_smap(sys_environ());
}

// i -> ()
void __sys_exit (Val code) {
  exit(code.i);
}

// () -> s
Val __sys_getLocale (void) {
  return (Val)sys_get_locale();
}

// () -> s
Val __sys_home (void) {
  return (Val)sys_user_home();
}

// () -> s
Val __sys_mainPath (void) {
  return __sys_mainPath_v;
}

// () -> i
Val __sys_pid (void) {
  return (Val)getpid();
}

// s -> ()
void __sys_printError (Val text) {
  fprintf(stderr, "%s", text.s);
}

// () -> s
Val __sys_readLine (void) {
  return (Val)sys_read_line();
}

// s -> ()
void __sys_setLocale (Val charset) {
  return sys_set_locale(charset.s);
}

// i -> ()
void __sys_sleep (Val millis) {
  sys_sleep(millis.i);
}

// () -> s
Val __sys_user (void) {
  return (Val)sys_user_name();
}
