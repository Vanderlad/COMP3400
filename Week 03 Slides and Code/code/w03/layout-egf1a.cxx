struct A {
  void setValue(int);   // *this is "A"
  int getValue() const; // *this is "A const"
  int getValue() volatile; // *this is "A volatile"
  int getValue() const volatile; 
    // *this is "A const volatile"
  std::string getString() const&; // *this is "A const&"
  std::string getString() &&; // *this is "A&&"
  // etc.
};
