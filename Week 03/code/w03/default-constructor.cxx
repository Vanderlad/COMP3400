struct Type 
{
  int* p;
  // ...
  Type() :            // invoke constructors and initialize members after : before {
    p(new int(42))    // construct int* with the result of new int(42)
  {                   // after { all members have been constructed
  }
  //...
};
