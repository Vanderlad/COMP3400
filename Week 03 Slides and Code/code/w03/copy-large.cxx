#include <iostream>
#include <algorithm>
using namespace std;
class A 
{
  double *ptr;
  public:
    A() : ptr(new double[100]) { cout << "A() " << this << '\n'; }
    A(A const& b) : ptr (new double[100]) {
      cout << "A(copy) " << this << '\n';
      std::copy(b.ptr, b.ptr+100, ptr); 
    }
    A& operator =(A const& b) { 
      cout << "a=b;(copy) " << this << '\n';
      std::copy(b.ptr, b.ptr+100, ptr);
      return *this;
    }
    ~A() { 
      cout << "~A() " << this << '\n'; 
      delete[] ptr; 
    }
    A operator +(A const& b) const {
      cout << "a+b; " << this << '\n';
      A retval(*this); // i.e., create local copy every invocation
      for (int i{}; i != 100; ++i)
        retval.ptr[i] += b.ptr[i];
      return retval;
    }
};

int main() { 
  A a, b, c;
  c = a + b + b;
}
