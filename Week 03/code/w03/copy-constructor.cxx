struct Type 
{
  int* p;
  // ...
  Type(Type const& t) :   // invoke constructors and initialize members after : before {
    p(new int(*t.p))      // construct int* with the result of new int(42)
  {                       // after { all members have been constructed
  }
  //...
};
