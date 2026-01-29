// program is p2i2r-7.cpp modified to support ranges...
#include <algorithm>
#include <iostream>
#include <ranges>     // For C++20 range-related items

struct mycontainer { int ar[5]; }; // a trivial "container" type to hold array
using myiterator = int*;
myiterator begin(mycontainer& c) { return c.ar; }
myiterator end(mycontainer& c) { return c.ar+5; }
static_assert(std::ranges::range<mycontainer>); // i.e., mycontainer is a (minimal) C++20 range

int main()
{
  mycontainer ar{ 2, 7, -3, 1, 4 };
  std::ranges::sort(ar);
  std::ranges::for_each(ar, [](int i) { std::cout << i << ' '; });
  std::cout << '\n';
}
