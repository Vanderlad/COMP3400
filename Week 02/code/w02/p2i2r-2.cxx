#include <iostream>

int main()
{
  using namespace std;

  int ar[] = { 1, 2, 3, 4, 5 };
  int *start = &ar[0];    // start points to a[0]
  int *stop = start + 5;  // start points to a[5] which is one past the last element

  for (int *i=start; i != stop; ++i)
    cout << *i << ' ';
  cout << '\n';
}
