struct foo 
{ 
  int i;

  auto operator+(foo const& rhs) const // argument is the second operand
                         // this object is the first operand
  {
    return foo{ this->i + rhs.i };
  }
};
