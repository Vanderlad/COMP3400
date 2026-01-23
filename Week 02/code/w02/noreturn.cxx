#include <cstdlib>
[[noreturn]] void foo() { throw "something"; }
[[noreturn]] void bar() { for (;;) ; }
[[noreturn]] void quit() { std::exit(10); }
