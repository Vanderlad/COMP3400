struct A { /* ... */ }; 
struct B : virtual A { /* ... */ };
struct C : virtual A, Y {  /* ... */ };
struct D : B, C { /* ... */ };
