#include <initializer_list>
#include <list>

struct my_list
{
  my_list(std::initializer_list<int> l) :
    l_(l.begin(), l.end())  
    // i.e., Pass iterators to list<int> constructor
  {
  }

  list<int> l_;
};

// Construct my_list with some ints...
my_list wow{ 1, -53, 3, 123, 45, 234 };
