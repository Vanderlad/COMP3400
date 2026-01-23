struct base {
  base() : b_(12) { }   // Construct b_ with 12.
  int b_;
};
struct derived {
  derived() : 
    base{}, d_(231)     // Default construct base; d_ = 231
  { }
  int d_;
};
void func(derived const&) { }
derived d;              // Default constructed
derived d2{};           // Default constructed
func(derived{});        // Unnamed and def. constructed.
// Next line is the "most vexing parse"...
derived d3();           // This is a function pointer declaration.
