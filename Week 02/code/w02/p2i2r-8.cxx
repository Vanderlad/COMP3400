#include <algorithm>
#include <iostream>
#include <vector>
  
int main()
{
  using namespace std;

  vector<int> ar{ 2,7,-3,1,4 }; // replace array literal with std::vector
  sort(begin(ar), end(ar)); // use std::begin() and std::end()
  for_each(begin(ar), end(ar), [](int i) { cout << i << ' '; }); // likewise
  cout << '\n';
}
