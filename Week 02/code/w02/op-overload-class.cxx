struct foo 
{ 
  int i;

  auto operator+() const // notice there is no argument
                         // the object itself is the first argument
  {
    return foo{ this->i+3 };
  }
};
