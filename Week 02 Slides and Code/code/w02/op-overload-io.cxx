#include <iostream>

struct foo { int i; };

std::ostream& operator<<(std::ostream& os, foo const& f) // i.e., notice const&
{
  os << f.i;
  return os;
}

std::istream& operator>>(std::istream& is, foo& f) // i.e., notice & and no const
{
  is >> f.i;
  return is;
}

int main() 
{ 
  foo a; 
  std::cin >> a;
  std::cout << "read in: " << a << '\n'; 
}

