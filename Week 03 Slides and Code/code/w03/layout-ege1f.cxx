// struct A, B, and C as before
struct D : B, C 
{
  D(int d) : 
    A(d*2+1), B(d*2), C(d/2), d_(d) 
  { 
  }
  int d;
};
