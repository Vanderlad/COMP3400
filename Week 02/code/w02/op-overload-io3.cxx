#include <istream>
#include <ostream>
#include <iostream>
#include <string>

struct foo { int i; };

std::ostream& operator<<(std::ostream& os, foo const& f) // i.e., notice const&
{
  os << "foo{ " << f.i << " }";
  return os;
}

std::istream& operator>>(std::istream& is, foo& f) // i.e., notice & and no const
{
  std::istream::sentry s(is, true); // true ensures whitespace is skipped
  if (s) // i.e., if the stream is in a good state
  {
    std::string before, after;
    int temp;
    if ((is >> before >> temp >> after) && before == "foo{" && after == "}")
      f.i = temp; // set f since temp was properly read in
    else
      is.setstate(std::ios_base::badbit); // temp not properly read in; make stream bad
  }
  return is;
}

int main() 
{ 
  foo a; 
  if (std::cin >> a) 
    std::cout << "read in: " << a << '\n'; 
}

