struct A { 
  A(int a) : a_(a) { } 
  int a_; 
};
struct B : virtual A { 
  B(int b) : A(b+1), b_(b) { }
  int b_; 
};
struct C : virtual A { 
  C(int c) : A(c-1), c_(c) { }
  int c_;
};
struct D : B, C {
  D(int d) : B(d*2), C(d/2), d_(d) { } // UH OH! ERROR!
  int d;
};
