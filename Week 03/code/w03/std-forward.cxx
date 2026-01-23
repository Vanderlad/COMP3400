#include <cmath>
#include <iostream>
#include "output.hxx"
using namespace std;

struct Foo 
{
  double d_;

  // Permit initialization of Foo instance with a double...
  explicit Foo(double d) : d_{d} { 
    cout << "Foo(" << d_ << ") " << this << '\n';
  }

  // Permit implicit cast to double...
  operator double() const { return d_; }
  // Constructors, assignment operators, and destructors w/outputs...
  Foo() : d_{} { OUT("Foo() "); }
  Foo(Foo const& f) : d_{f.d_} { OUT2("Foo(copy) ",&f); }
  Foo(Foo&& f) : d_{f.d_} { OUT2("Foo(move) ",&f); }
  Foo& operator =(Foo const& f) { 
    OUT2("f=g;(copy) ",&f); 
    d_ = f.d_; return *this;
  }
  Foo& operator =(Foo&& f) { 
    OUT2("f=g;(move) ",&f); 
    d_ = f.d_; return *this;
  }
  ~Foo() { OUT("~Foo() "); }
};
template <typename Op, typename Arg>
constexpr auto simple_proxy(Op&& op, Arg&& arg) {
  return op(std::forward<Arg>(arg));
}
template <typename Op, typename... Args>
constexpr auto fancy_proxy(Op&& op, Args&&... args) {
  return op(std::forward<Args>(args)...);
}
constexpr double my_func(double a, double b) { return a+b; }

int main() {
  cout << "sp:\n"; 
  cout << simple_proxy<double(double)>(std::sin, Foo{0.0}) << "\n";
  cout << "\nfp:\n"; 
  cout << fancy_proxy(my_func, Foo{1.1}, Foo{2.2}) << '\n';
}
