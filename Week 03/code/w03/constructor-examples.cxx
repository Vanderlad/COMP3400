#include <utility>
struct Type 
{
  int* p;

  Type() : p(new int(42)) { }                     // default constructor
  Type(int i) : p(new int(i)) { }                 // a constructor
  Type(Type const& t) : p(new int(*t.p)) { }      // copy constructor
  Type(Type&& t) : p(t.p) { t.p = nullptr; }      // move constructor

  ~Type() { delete p; }                           // destructor
};

Type a;                                           // a is default constructed
Type b=a, c(a);                                   // b and c are copy constructed
Type d=std::move(a), e(std::move(b)), f(Type());  // d, e, and f are move constructed
Type y(59), z(60);                                // y and z invoke the int constructor
