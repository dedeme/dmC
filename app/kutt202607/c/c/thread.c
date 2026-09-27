
// thread ------------------------------

// <thread> -> ()
void __thread_join (Val th) {
  thread_join(th.o);
}

// (->()) -> ()
void __thread_run (Val fn) {
  thread_run ((void (*)(void))fn.o);
}

// (->()) -> <thread>
Val __thread_start (Val fn) {
  return (Val)(void *)thread_start((void (*)(void))fn.o);
}

// (->()) -> ()
void __thread_sync (Val fn) {
  thread_sync ((void (*)(void))fn.o);
}
