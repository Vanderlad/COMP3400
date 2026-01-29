#include <iostream>

using myiterator = int*;
myiterator mybegin(int* startpos) { return startpos; }  // no longer hard-coded to ar
myiterator myend(int* stoppos) { return stoppos+5; }    // hard-coded to ar's size

int main()
{
  using namespace std;

  int ar[] = { 1, 2, 3, 4, 5 };
  for (myiterator i=mybegin(ar); i != myend(ar); ++i) // pass in ar array
    cout << *i << ' ';
  cout << '\n';
}
