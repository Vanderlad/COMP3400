struct Type 
{
  int* p;
  // ...
  Type(Type&& t) :        // invoke constructors and initialize members after : before {
    p(t.p)                // construct int* with the result of new int(42)
  {                       // after { all members have been constructed
    t.p = nullptr;        // t refers to an object that still needs to be destroyed!
  }
  //...
};
