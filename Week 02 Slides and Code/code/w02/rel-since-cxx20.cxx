#include <compare>      // needed for std::*_ordering definitions

// for some user-defined type, e.g.,...
struct foo 
{ 
  int i; 

  // One can instead write a friend operator<=>(const&, const&) "= default" 
  // definition which will (lexicographically) define <=>, ==, !=, <, <=, >=, 
  // and >
  friend constexpr auto operator<=>(foo const&, foo const&) noexcept = default;
};

int main()
{
  foo a{1}, b{2};
  if (a < b) { /* ... */ }
}
