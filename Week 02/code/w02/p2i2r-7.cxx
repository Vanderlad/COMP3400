#include <algorithm>
#include <iostream>

using myiterator = int*;
myiterator mybegin(int* startpos) { return startpos; }
myiterator myend(int* stoppos) { return stoppos+5; }

int main()
{
  using namespace std;

  int ar[] = { 2, 7, -3, 1, 4 };
  sort(mybegin(ar), myend(ar));
  for_each(mybegin(ar), myend(ar), [](int i) { cout << i << ' '; });
  cout << '\n';
}
