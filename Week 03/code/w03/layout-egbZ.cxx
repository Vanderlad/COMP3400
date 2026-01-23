// struct X from previous example
// struct Y from previous example
struct Z : Y
{
  int z;
  void h(float) override;
  virtual void r(Z&);
};
