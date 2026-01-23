#include <iostream>

template <typename T>
void print(T&& t) // T&& here matches ALL types incl. any ref and/or cv-qualifiers!
{
  std::cout << t << std::endl;
}

int main()
{
  int i = 65;
  print(5.5); print("Hello"); print(i); print(char('A'));
}
