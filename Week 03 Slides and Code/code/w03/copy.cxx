#include <iostream>
#include "output.hxx"
class A {
  public:
    A() { OUT("A() "); }
    A(A const& b) { OUT2("A(copy) ",&b); }
    ~A() { OUT("~A() "); }
};

A a_function() {
  A a; // default construct A
  return a; // return copy of A
}

A value = a_function(); // Copy elision possible here.

int main() { }
