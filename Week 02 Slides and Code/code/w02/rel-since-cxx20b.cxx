#include <compare>
#include <iostream>
struct foo 
{ 
  int i; 
  // operator== must be written since <=> is not =default...
  friend bool operator==(foo const& lhs, foo const& rhs) = default;
  friend constexpr auto operator<=>(foo const& lhs, foo const& rhs) noexcept
  {
#if 1
    return lhs.i <=> rhs.i; // use built-in operator<=>(int,int) definition
#else // or do this more manually...
    if (lhs.i < rhs.i)
      return std::strong_ordering::less;
    else if (lhs.i == rhs.i)
      return std::strong_ordering::equal;
    else
      return std::strong_ordering::greater;
#endif
  }
};

int main()
{
  foo a{1}, b{2};
  std::cout << (a == b) << (a != b) << (a < b) << (a <= b) << (a >= b) << (a > b) << '\n' 
    // besides directly comparing ordering values, orderings can be compared 
    // relationally to 0, e.g.,
    << ((a <=> b) == 0) << ' ' << ((a <=> b) < 0) << '\n';
}
