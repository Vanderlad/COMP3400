#include <istream>
#include <ostream>
#include <iostream>
#include <string>

struct foo { int i; };

std::ostream& operator<<(std::ostream& os, foo const& f) // i.e., notice const&
{
  os << '[' << f.i << ']';
  return os;
}

std::istream& operator>>(std::istream& is, foo& f) // i.e., notice & and no const
{
  if (is.get() != '[')
  {
    is.unget(); // put character read in back in stream buffer
    is.setstate(std::ios_base::failbit); // fail the stream
    return is;
  }

  int temp;
  if (is >> temp && is.get() == ']')
    f.i = temp;
  else
    is.setstate(std::ios_base::badbit); // make stream bad
  return is;
}

int main() 
{ 
  foo a; 
  if (std::cin >> a) 
    std::cout << "read in: " << a << '\n'; 
}

