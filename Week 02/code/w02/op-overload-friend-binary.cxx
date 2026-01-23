struct foo 
{ 
  int i;

  friend auto operator+(foo const& a, foo const& b)
  {
    return foo{ a.i + b.i };
  }
};
