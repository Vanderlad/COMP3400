//Assignment 3 Vlad Mihaescu, 110014634

#include <algorithm> // e.g., for std::next_permutation()
#include <cstddef> // e.g., for std::size_t
#include <generator> // e.g., for std::generator
#include <iostream> // e.g., for std::cin and std::cout
#include <iterator> // e.g., for std::back_inserter()
#include <limits> // e.g., for std::numeric_limits<T>
#include <stdexcept> // e.g., for  std::length_error
#include <vector> // e.g., for std::vector

template <typename R>
concept ForwardSizedRange =
  requires(R& r)
  {
    r.begin();
    r.end();
    r.size();
    requires std::forward_iterator<decltype(r.begin())>;
  };

template <ForwardSizedRange R>
auto all_subsets(R&& r)
  -> std::generator<std::vector<decltype(r.begin())>>
{
    using iter_t = decltype(r.begin());
    using T = std::vector<iter_t>;

    std::size_t const n{ r.size() };

    if (n >= std::numeric_limits<std::size_t>::digits)
        throw std::length_error("range too large");

    std::size_t const num_subsets{ std::size_t(1) << n };

    // build "all" = iterators to each element in r
    T all;
    all.reserve(r.size());

    auto in = r.begin();
    auto in_end = r.end();
    auto out = std::back_inserter(all);

    for (; in != in_end; ++in)
        *out = in;

    // reusable output vector for a subset
    T v;
    v.reserve(r.size());

    // yield all subsets with bitmask
    for (std::size_t i = 0; i < num_subsets; ++i)
    {
        v.clear();

        for (std::size_t j = 0; j < n; ++j)
        {
            if ( ( (i >> j) & 1u ) != 0u )
                v.push_back(all[j]);
        }

        co_yield v;
    }
}


int main()
{
  // read in all int values from stdin into a vector...
  std::vector<int> v;
  for (int i; std::cin >> i; )
    v.push_back(i);

  // output all permutations of that vector...
  auto gr{ all_subsets(v) }; // returns std::generator< /* vector of iterators */ >
  auto f{ gr.begin() }; // returns iterator to std::vector< /* iterator */ >
  auto l{ gr.end() }; // returns "end" iterator / sentinel relative to f
  for (; f != l; ++f)
  {
    // f is an iterator to std::vector of iterators
    // So *f is a std::vector of iterators.
    // So the elements of *f are all iterators.
    // So *element is needed to access the element.
    std::cout << "SUBSET: ";
    for (auto const& elem : *f)
      std::cout << *elem << ' ';
    std::cout << '\n';
  }
}