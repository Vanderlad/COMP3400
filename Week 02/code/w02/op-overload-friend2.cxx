struct foo; // forward declaration
auto operator+(foo const&); // function prototype

struct foo 
{ 
  int i;
  friend auto operator+(foo const&); // friend declaration
};

auto operator+(foo const& a) // definition
{
  return foo{ a.i+3 };
}
