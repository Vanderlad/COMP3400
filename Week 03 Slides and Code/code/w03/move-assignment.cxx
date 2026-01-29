#include <utility>
struct Type {
  int* p;
  Type() : p{new int{}} { }
  Type(int const& i) : p{new int{i}} { }
  Type(Type&& t) : p{std::move(t.p)} { }
  Type& operator =(Type&& t)        // move assignment operator
  {
    Type local(std::move(t));       // move t into *this
    std::swap(p,local.p);           // exchange pointers
    return *this;                   // local will now be destroyed
  }
};
int main()
{
  Type a(11), b{12}, c = 13;        // a, b, and c invoke Type(int const&) constructor
  a = std::move(c); b = Type{};     // move assignments
}
