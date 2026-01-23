#include <initializer_list>
#include <iostream>
#include <vector>

// Invokes std::vector<int>(std::initializer_list<int>)...
std::vector<int> v{ 1, 5, 23, -123, 45, 34 56 };
template <typename T>
T sum(std::initializer_list<T> l)
{
  T retval{};
  for (auto const& element : l)
    retval += element;
  return retval;
}
// ...
std::cout << sum({1, 2, 3}) << '\n'; // Outputs: 6
