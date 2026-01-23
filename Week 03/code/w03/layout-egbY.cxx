// struct X from previous example
struct Y : X
{
  int y;
  void g(char) override;
  virtual void q(Y const&);
};
