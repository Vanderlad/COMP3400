// Vlad Mihaescu
//SN: 110014634

#include <cstdint>   // std::intmax_t
#include <tuple>     // std::tuple
#include <map>       // std::map
#include <iostream>  // std::cout

class ackermann
{
private:
  using ret_type   = std::intmax_t;
  using args_type  = std::tuple<std::intmax_t, std::intmax_t>;
  using cache_type = std::map<args_type, ret_type>;

  static cache_type cache_;

public:
  ret_type operator()(std::intmax_t const m, std::intmax_t const n) const
  {
    // Check cache first
    const args_type key{m, n};
    auto it = cache_.find(key);
    if (it != cache_.end())
      return it->second;

    // Compute Ackermann
    ret_type result = 0;

    if (m == 0)
    {
      result = n + 1;
    }
    else if (n == 0)
    {
      result = (*this)(m - 1, 1);
    }
    else
    {
      ret_type inner = (*this)(m, n - 1);
      result = (*this)(m - 1, inner);
    }

    // Memoize and return
    cache_[key] = result;
    return result;
  }
};

// Definition of the static cache
ackermann::cache_type ackermann::cache_;

int main()
{
  using namespace std;

  ackermann a;
  for (int m = 0; m != 5; ++m)
  {
    cout << "m = " << m << ": ";
    intmax_t last_result = 0;
    for (int n = 0; n != 10 && last_result < 65533; ++n)
    {
      last_result = a(m, n);
      cout << last_result << ' ';
    }
    cout << '\n';
  }
}