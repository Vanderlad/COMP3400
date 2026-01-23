struct Type 
{
  int* p;
  // ...
  ~Type() 
  { 
    delete p;                                     // free memory associated with p
  }
  // ...
};
