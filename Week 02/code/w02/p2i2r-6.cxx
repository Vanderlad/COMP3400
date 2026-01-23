#include <algorithm>
#include <iostream>

using myiterator = int*;
myiterator mybegin(int* startpos) { return startpos; }
myiterator myend(int* stoppos) { return stoppos+5; }

int main()
{
  using namespace std;
  int ar[] = { 1, 2, 3, 4, 5 };
  for_each( mybegin(ar), myend(ar), // begin at ar[0] and process up to but not including ar[5]
    [](int i) { cout << i << ' '; } // lambda function
  );
  cout << '\n';
}
