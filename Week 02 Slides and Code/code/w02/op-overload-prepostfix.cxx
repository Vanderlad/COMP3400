struct foo
{
  int i;

  foo& operator++()
  {
    ++this->i;
    return *this;
  }

  foo operator++(int) // postfix operator
  {
    foo const retval{*this}; // make a copy
    this->operator++(); // call prefix operator on *this
    return retval; // return the copy
  }
};
