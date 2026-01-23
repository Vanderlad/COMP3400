struct node           // no inheritance
{
  void add(char);     // not virtual
  char data_[10];
  void remove(char);  // not virtual
  node *next_;
};
