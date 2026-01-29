#include <iostream>
#include "output.hxx"
class A {
public:
  A() { OUT("A() "); }
  A(A const& b) { OUT2("A(copy) ",&b); }
  A(A&& b) { OUT2("A(move) ",&b); }
  A& operator =(A const& b) { OUT2("a=b;(copy) ",&b); return *this; }
  A& operator =(A&& b) { OUT2("a=b;(move) ",&b); return *this; }
  ~A() { OUT("~A() "); }
};
int main() { 
  A a, b; // default constructions
  A c = A{}, d{std::move(a)}, e = std::move(b); // move constructions
  a = b; // copy assignment
  a = std::move(b); a = A{}; // move assignment
}
