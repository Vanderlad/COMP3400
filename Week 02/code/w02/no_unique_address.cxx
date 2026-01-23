struct foo { }; // i.e., foo is an empty class
struct bar1
{ 
  int i; 
  [[no_unique_address]] foo f; 
  int j;
};
struct bar2 { int i; int j; };

static_assert(sizeof(foo) >= 1); // the size of any object must at least be one
static_assert(sizeof(bar1) == sizeof(bar2));
