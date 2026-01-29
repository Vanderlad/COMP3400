#include <iostream>
int main() 
{
  if (int i; std::cin >> i)
  {
    switch (i)
    {
      case 1:
      case 2:
        std::cout << "1 or 2\n";
        [[fallthrough]];
      case 3: // no warning given here
        std::cout << "3\n";
    }
  }
}
