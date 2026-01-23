struct W
{
  int w;
  virtual void f(); // F1
  void f(long); // F2
  virtual void f(int); // F3
  virtual void f(double); // F4
};
