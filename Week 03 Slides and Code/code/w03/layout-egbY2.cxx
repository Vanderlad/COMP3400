// struct X from previous example
struct Y2 : X
{
  int y2;
  using X::g; // Unhide X members named g
  virtual void g(double); // F0. Different signature!
};
