struct foo 
{ 
  int i;

  friend auto operator+(foo const& a)
  {
    return foo{ a.i+3 };
  }
};
