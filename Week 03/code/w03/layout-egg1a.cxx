class abstract
{
  // Always have a virtual destructor in an
  // abstract/polymorphic class...
  virtual ~abstract() { }

  // Pure virtual (abstract) function...
  virtual int move(int x, int y) = 0;
};
