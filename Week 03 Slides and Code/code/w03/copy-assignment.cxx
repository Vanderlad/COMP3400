#include <utility>                  // for std::swap since C++11, <algorithm> before C++11
struct Type {
  int* p;
  Type(int const& i) : p(new int{i}) { }
  Type(Type const& t) : p(new int{*t.p}) { }
  Type& operator =(Type const& t)   // copy assignment operator
  {
    Type local(t);                  // create a local copy of t
    std::swap(p,local.p);           // exchange pointers
    return *this;                   // local will now be destroyed
  }
};
int main()
{
  Type a(42), b{52}, c = 62;        // a and b invoke Type(int const&) constructor
  a = b = c;                        // copy assignment
}
