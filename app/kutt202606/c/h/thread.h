
// thread ------------------------------

// <thread> -> ()
void __thread_join (Val th);

// (->()) -> ()
void __thread_run (Val fn);

// (->()) -> <thread>
Val __thread_start (Val fn);

// (->()) -> ()
void __thread_sync (Val fn);
