#include <iostream>

int ar[] = { 1, 2, 3, 4, 5 };

using myiterator = int*;                // same as: typedef int* myiterator;
myiterator start() { return &ar[0]; }   // hard-coded to ar array for now
myiterator stop() { return start()+5; } // hard-coded to ar array for now

int main()
{
  using namespace std;
  for (myiterator i=start(); i != stop(); ++i) // now use myiterator, start(), and stop()
    cout << *i << ' ';
  cout << '\n';
}
