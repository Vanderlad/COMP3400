#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
  using namespace std;
  vector ar{ 2, 7, -3, 1, 4 };
  ranges::sort(ar);
  ranges::for_each(ar, [](int i) { cout << i << ' '; });
  cout << '\n';
}
