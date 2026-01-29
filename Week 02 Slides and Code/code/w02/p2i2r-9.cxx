#include <algorithm>
#include <execution> // For C++17 parallel algorithm's execution policies.
#include <vector>
#include <iostream>

using namespace std;
  
int main()
{
  vector<int> ar{ 2, 7, -3, 1, 4 };
  sort(execution::par, begin(ar), end(ar)); // parallel
  for_each(execution::par, begin(ar), end(ar), [](int i) { cout << i << ' '; }); // parallel
  cout << '\n';
}

