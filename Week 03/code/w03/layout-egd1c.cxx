D d;
d.z = 1; // ERROR! Ambiguous!
d.B::z = 1; // Okay. Unique via B.
d.C::z = 2; // Okay. Unique via C.
d.y = 3; // Okay. Unambiguous.
