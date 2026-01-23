struct foo { int i; };

auto operator+(foo const& a)
{
  return foo{ a.i+3 };
}
