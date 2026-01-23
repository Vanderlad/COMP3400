struct Z { int z; };
struct Y { int y; };
struct A : Z { int a; };
struct B : A { int b; };
struct C : A, Y { int c; };
struct D : B, C { int d; };
