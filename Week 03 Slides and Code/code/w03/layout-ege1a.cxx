// Only A is inherited virtually below!
struct Z { int z; };
struct Y { int y; };
struct A : Z { int a; };
struct B : virtual A { int b; };
struct C : virtual A, Y { int c; };
struct D : B, C { int d; };
