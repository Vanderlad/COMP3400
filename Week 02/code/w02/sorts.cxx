#include <algorithm>
#include <compare>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <iostream>
#include <forward_list>
#include <functional>
#include <list>
#include <numeric>
#include <print>
#include <ranges>
#include <random>
#include <string>
#include <vector>

//=============================================================================

//
// std::permutable<I> is defined as...
//   template <typename I>
//   concept permutable =
//     std::forward_iterator<I> &&
//     std::indirectly_movable_storable<I, I> &&
//     std::indirectly_swappable<I, I>
//   ;
// The names *_permutable_c are defined in this file for ease-of-use below.
//

template <typename I>
concept forward_permutable_c = std::permutable<I>;

template <typename I>
concept bidirectional_permutable_c = std::bidirectional_iterator<I> &&
  forward_permutable_c<I>;

template <typename I>
concept random_access_permutable_c = std::random_access_iterator<I> &&
  bidirectional_permutable_c<I>;

template <typename I>
concept contiguous_permutable_c = std::contiguous_iterator<I> &&
  random_access_permutable_c<I>;

//=============================================================================

// NOTE: one_position_before_end can be the end if [first,last) is empty.

template <std::bidirectional_iterator It>
constexpr auto one_position_before_end(It first, It last)
{
  if (first == last) return first;
  else return --last;
}

template <std::forward_iterator It, std::sized_sentinel_for<It> Sent>
constexpr auto one_position_before_end(It first, Sent last)
{
  if (first == last) return first;
  std::ranges::advance(first,last-first-1,last);
  return first;
}

template <std::forward_iterator It, std::sentinel_for<It> Sent>
constexpr auto one_position_before_end(It first, Sent last)
{
  auto prev{ first };
  for (; first != last; ++first)
    prev = first;
  return prev;
}

constexpr auto one_position_before_end(std::ranges::range auto&& r)
{
  return one_position_before_end(r.begin(),r.end());
}

//=============================================================================

template <std::ranges::forward_range R>
constexpr auto one_position_before_midpoint(R&& r) ->
  std::ranges::iterator_t<R>
{
  using std::ranges::advance;
  using std::ranges::next;

  auto const last{ r.end() };
  auto slow_iter{ r.begin() };
  auto fast_iter{ next(slow_iter,1,last) };
  if (fast_iter != last)
  {
    advance(fast_iter,1);
    while (fast_iter != last)
    {
      advance(slow_iter,1);
      advance(fast_iter,1);
      if (fast_iter == last)
        break;
      advance(fast_iter,1);
    }
  }
  return slow_iter;
}

template <
  std::forward_iterator FwdIter,
  std::sized_sentinel_for<FwdIter> Sentinel
>
constexpr auto one_position_before_midpoint(FwdIter first, Sentinel last) ->
  FwdIter
{
  using std::ranges::distance;
  using std::ranges::next;

  if (first == last)
    return first;
  else
  {
    // This code is more complicated to address the following:
    //   * avoid overflow when distance([first,last)) is the maximum
    //     value of the numeric type used to represent distances
    //   * compilers will typically optimize /2 and %2 code and it is
    //     also possible the resulting code is branch-free
    auto const dist{ distance(first,last) };
    return next(first, dist/2+(dist % 2 == 1)-1, last);
  }
}

//=============================================================================

template <std::bidirectional_iterator It>
constexpr auto position_at_end(It first, It last)
{
  if (first == last)
    return first;
  else
    return --last;
}

template <std::forward_iterator It, std::sized_sentinel_for<It> Sent>
constexpr auto position_at_end(It first, Sent last)
{
  if (first == last)
    return first;
  std::ranges::advance(first,last-first,last);
  return first;
}

template <std::forward_iterator It, std::sentinel_for<It> Sent>
constexpr auto position_at_end(It first, Sent last)
{
  for (; first != last; ++first)
    ;
  return first;
}

template <std::ranges::range R>
constexpr auto positition_at_end(R&& r)
{
  return position_at_end(r.begin(),r.end());
}

//=============================================================================

template <std::ranges::forward_range R>
constexpr auto midpoint(R&& r) ->
  std::ranges::iterator_t<R>
{
  using std::ranges::advance;

  auto const last{ r.end() };
  auto slow_iter{ r.begin() };
  auto fast_iter{ slow_iter };
  if (fast_iter != last)
  {
    advance(fast_iter,1);
    while (fast_iter != last)
    {
      advance(slow_iter,1);
      advance(fast_iter,1);
      if (fast_iter == last)
        break;
      advance(fast_iter,1);
    }
  }
  return slow_iter;
}

template <
  std::forward_iterator FwdIter,
  std::sized_sentinel_for<FwdIter> Sentinel
>
constexpr auto midpoint(FwdIter first, Sentinel last) ->
  FwdIter
{
  using std::ranges::distance;
  using std::ranges::next;
  return next(first, distance(first,last)/2, last);
}

#if 0
// This version is not as good. Why?
//   * optimal for O(1)-style random-access containers
//   * for other containers distance touches all nodes in the container
//     and then next traverses half of those nodes to get to the desired
//     node
// The above versions are better since:
//   * if the range has O(1) size then only the elements that need
//     to be traversed are
//   * otherwise Floyd's cycle finding technique of using slow and 
//     fast iteration is used to determine the midpoint without 
//     calculating any value
//       * this does touch all nodes but one really has no choice
//       * since no size is ever determined there are no worries
//         about overflow
//       * that said this does require the range to be a forward
//         range since slow will always be visiting a node that
//         fast was at
template <std::ranges::range R>
constexpr auto midpoint(R&& r) ->
  std::ranges::iterator_t<R>
{
  return std::ranges::next(r.begin(),std::ranges::distance(r)/2,r.end());
}
#endif

//=============================================================================

//
// convenience functions that are used to determine whether or not
// a range is empty or has one element...
//
template <std::forward_iterator FwdIt, std::sentinel_for<FwdIt> Sent>
constexpr bool is_range_size_zero_or_one(FwdIt first, Sent last)
{
  return first == last || std::ranges::next(first) == last;
}

template <std::ranges::forward_range R>
constexpr bool is_range_size_zero_or_one(R&& r)
{
  return is_range_size_zero_or_one(r.begin(), r.end());
}

//=============================================================================

// bubble_sort()
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Bubble_sort
//   * O(n**2)

template <forward_permutable_c FwdIt, std::sentinel_for<FwdIt> ItSent, 
  typename Compare = std::ranges::less>
requires std::sortable<FwdIt,Compare>
constexpr void bubble_sort(FwdIt first, ItSent last, Compare comp = {})
{
  using std::ranges::iter_swap;
  using std::ranges::next;

  if (is_range_size_zero_or_one(first,last))
    return;

  // [first,last) has at least 2 elements...
  //   * this means prev_i and i are initially valid
  bool swapped;
  do
  {
    swapped = false;
    for (auto prev_i{ first }, i{ next(first) }; i != last; (void)++prev_i, (void)++i)
    {
      if (comp(*i,*prev_i))
      {
        iter_swap(i,prev_i);
        swapped = true;
      }
    }
  }
  while (swapped);
}

template <std::ranges::forward_range R, typename Compare = std::ranges::less>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void bubble_sort(R&& r, Compare cmp = {})
{
  bubble_sort(r.begin(),r.end(),cmp);
}

//=============================================================================

// selection_sort()
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Selection_sort
//   * O(n**2)

template <forward_permutable_c FwdIt, std::sentinel_for<FwdIt> ItSent, 
  typename Compare = std::ranges::less>
requires std::sortable<FwdIt,Compare>
constexpr void selection_sort(FwdIt first, ItSent last, Compare cmp = {})
{
  using std::ranges::iter_swap;
  using std::ranges::min_element;

  for (; first != last; ++first)
    iter_swap(first, min_element(first,last,cmp));
}

template <std::ranges::forward_range R, typename Compare = std::ranges::less>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void selection_sort(R&& r, Compare cmp = {})
{
  selection_sort(r.begin(),r.end(),cmp);
}

//=============================================================================

// insertion_sort()
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Insertion_sort
//   * O(n**2)

template <bidirectional_permutable_c BidiIt, std::sentinel_for<BidiIt> ItSent, 
  typename Compare = std::ranges::less>
requires std::sortable<BidiIt,Compare>
constexpr void insertion_sort(BidiIt first, ItSent last, Compare cmp = {})
{
  using std::ranges::iter_swap;
  using std::ranges::prev;

  for (auto i{ first }; i != last; ++i)
    for (auto j{ i }; j != first && cmp(*j,*prev(j)); --j)
      iter_swap(prev(j),j);
}

template <
  std::ranges::bidirectional_range R, 
  typename Compare = std::ranges::less
>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void insertion_sort(R&& r, Compare cmp = {})
{
  insertion_sort(r.begin(),r.end(),cmp);
}

//=============================================================================

// shell_sort()
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Shellsort
//   * This version uses a small hard-coded Ciura gap sequence from the
//     Wikipedia article. In general one can calculate such values
//     as needed, however, since the last value is 1 it will always 
//     result in sorted data.
//   * Unlike the Wikipedia article and many textbooks this routine calls
//     insertion_sort() to perform the insertion rather than hard-code
//     embed the insertion sort into this sort.

template <std::ranges::bidirectional_range R, typename Compare = std::ranges::less>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void shell_sort(R&& r, Compare cmp = {})
{
  using namespace std::ranges;

  if (is_range_size_zero_or_one(r))
    return;

  std::initializer_list<std::size_t> const il{ 
    701, 301, 132, 57, 23, 10, 4, 1 
  };

  // Set skip_by to the first position in il that is equal to or smaller
  // than the size of the range only if size is O(1). Otherwise set to 
  // il.begin().
  auto skip_by = 
    [&]()
    {
      if constexpr(sized_range<R>)
        return lower_bound(il, std::ranges::size(r), greater{});
      else
        return il.begin();
    }()
  ;
  
  for (; skip_by != il.end(); ++skip_by)
  {
    auto gap_view{ r | std::views::stride(*skip_by) };
    insertion_sort(gap_view.begin(), gap_view.end(), cmp);
  }
}

template <
  bidirectional_permutable_c It,
  std::sentinel_for<It> ItSent,
  typename Compare = std::ranges::less
>
requires std::sortable<It,Compare>
constexpr void shell_sort(It first, ItSent last, Compare cmp = {})
{
  shell_sort(std::ranges::subrange(first,last), cmp);
}

//=============================================================================

// heap_sort()
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Heapsort
//   * implemented in terms of algorithms in <algorithm>

template <random_access_permutable_c It, std::sentinel_for<It> Sent,
  typename Compare = std::ranges::less>
requires std::sortable<It,Compare>
constexpr void heap_sort(It first, Sent last, Compare = {})
{
  using namespace std::ranges;
  make_heap(first,last);
  sort_heap(first,last);
}

template <std::ranges::random_access_range R, typename Compare = std::ranges::less>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void heap_sort(R& r, Compare cmp = {})
{
  heap_sort(r.begin(),r.end(),cmp);
}

//=============================================================================

// quick_sort()...
//   * Wikipedia URL: https://en.wikipedia.org/wiki/Quicksort
//   * Average case: O(n*lg(n)), Worst cast: O(n**2)
//   * Since the quick_sort() is not stable three is little value in
//     using this code for non-random-access ranges relative to other
//     algorithm choices.

template <forward_permutable_c It, std::sentinel_for<It> ItSent,
  typename Compare = std::ranges::less>
requires std::sortable<It,Compare> 
constexpr void quick_sort(It first, ItSent last, Compare cmp = {})
{
  using namespace std::ranges;

  if (first == last) return;

  // use the last element as the pivot...
  auto pivot{ one_position_before_end(first,last) };
  auto mid{ midpoint(first,next(pivot)) };
  if (pivot != mid)
    iter_swap(pivot,mid); // swap elements

  auto second_part{
    // partition [first, prev(last)) using *pivot...
    partition(first, pivot, [&](auto const& v) { return cmp(v,*pivot); })
  };

  // place the pivot value at second_part.begin()...
  // (ASIDE: the pivot value is between the two halves.)
  iter_swap(pivot,second_part.begin());

  quick_sort(first, second_part.begin(), cmp);
  quick_sort(next(second_part.begin()), last, cmp);
}

template <bidirectional_permutable_c It, std::sentinel_for<It> ItSent,
  typename Compare = std::ranges::less>
requires std::sortable<It,Compare> 
constexpr void quick_sort(It first, ItSent last, Compare cmp = {})
{
  using namespace std::ranges;

  if (first == last) return;

  // NOTE: This requires bidirectional iterators since the previous
  //       position to partition's returned iterator is needed.
  auto mid{ midpoint(first,last) };
  iter_swap(first,mid);
  auto second_part{ partition(next(first),last,
    [&](auto const& v) { return cmp(v,*first); }) };
  auto previous{ prev(second_part.begin()) };
  if (first != previous)
    iter_swap(first,previous);

  quick_sort(first, previous, cmp);
  quick_sort(second_part.begin(), last, cmp);
}

template <
  std::ranges::forward_range R, 
  typename Compare = std::ranges::less
>
requires std::sortable<std::ranges::iterator_t<R>,Compare>
constexpr void quick_sort(R& r, Compare cmp = {})
{
  quick_sort(r.begin(),r.end(),cmp);
}

//=============================================================================

//
// Three-way partition of [first,last) is defined as follows:
//   * [first,lo) contains all elements less than the pivot
//   * [lo,i) contains all elements equal to the pivot
//   * [i,hi) contains all unpartitioned elements
//   * [hi,last) contains all elements greater than the pivot
//
template <bidirectional_permutable_c It, std::sentinel_for<It> ItSent>
requires std::three_way_comparable<std::iter_value_t<It>, std::weak_ordering>
constexpr auto three_way_partition(
  It first, ItSent last, std::iter_value_t<It> const& value
) -> std::tuple<It,It>
{
  using namespace std::ranges;

  if (first == last)
    return { first, first };

  auto lo{ first };
  auto hi{ position_at_end(first,last) };
  for (auto i{ lo }; i != hi; )
  {
    auto const cmp_result{ *i <=> value };
    if (cmp_result < 0)
      iter_swap(lo++,i++);
    else if (cmp_result > 0)
      iter_swap(i,hi--);
    else
      ++i;
  }
  return { lo, hi };
}

template <std::ranges::bidirectional_range R>
requires 
  std::three_way_comparable<
    std::ranges::range_value_t<R>,
    std::weak_ordering
  >
constexpr auto three_way_partition(R&& r, 
  std::ranges::range_value_t<R> const& value
) -> 
  std::tuple<
    std::ranges::iterator_t<R>,
    std::ranges::iterator_t<R>
  >
{
  return three_way_partition(r.begin(), r.end(), value);
}

//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================

template <template <typename> class TT>
void test_one_position_before_midpoint(std::size_t n)
{
  using namespace std;
  
  TT<size_t> c(n);
  iota(c.begin(), c.end(), size_t{1});
  auto pos{ one_position_before_midpoint(c) };
  auto const name{ typeid(c).name() };

  cout << "With type: " << name << '\n';
  cout << __func__ << "(" << n << "): before midpoint: ";
  for (auto i : std::ranges::subrange(c.begin(), pos))
    cout << i << ' ';
  cout << '\n';

  if (pos != c.end())
    cout << __func__ << "(" << n << "): midpoint " << *pos << '\n';

  cout << __func__ << "(" << n << "): after midpoint: ";
  for (auto i : std::ranges::subrange(std::ranges::next(pos,1,c.end()),c.end()))
    cout << i << ' ';
  cout << '\n';
}

template <template <typename> class TT>
void test_midpoint(std::size_t n)
{
  using namespace std;
  
  TT<size_t> c(n);
  iota(c.begin(), c.end(), size_t{1});
  auto pos{ midpoint(c) };
  auto const name{ typeid(c).name() };

  cout << "With type: " << name << '\n';
  cout << __func__ << "(" << n << "): before midpoint: ";
  for (auto i : std::ranges::subrange(c.begin(), pos))
    cout << i << ' ';
  cout << '\n';

  if (pos != c.end())
    cout << __func__ << "(" << n << "): midpoint " << *pos << '\n';

  cout << __func__ << "(" << n << "): after midpoint: ";
  for (auto i : std::ranges::subrange(std::ranges::next(pos,1,c.end()),c.end()))
    cout << i << ' ';
  cout << '\n';
}

int main()
{
  {
    for (auto i : { 0, 1, 2, 3, 4, 10, 11, 12 })
    {
      test_midpoint<std::forward_list>(i);
      std::cout << '\n';

      test_midpoint<std::list>(i);
      std::cout << '\n';
    
      test_midpoint<std::vector>(i);
      std::cout << '\n';
    }
  }
  {
    using namespace std;

    random_device rd;
    seed_seq ss{ rd(), rd(), rd(), rd(), rd() };
    default_random_engine re(ss);
    uniform_int_distribution<size_t> ud(0,1000);

    vector<size_t> v(10);
    std::ranges::generate_n(v.begin(), v.size(), [&]() { return ud(re); });
    println("{}", v);

    {
      auto t{v};
      bubble_sort(t);
      println("bubble_sort result: {}", t);
    }
    {
      auto t{v};
      selection_sort(t);
      println("selection_sort result: {}", t);
    }
    {
      auto t{v};
      insertion_sort(t);
      println("insertion_sort result: {}", t);
    }
    {
      auto t{v};
      shell_sort(t);
      println("shell_sort result: {}", t);
    }
    {
      auto t{v};
      heap_sort(t);
      println("heap_sort result: {}", t);
    }
    {
      auto t{v};
      quick_sort(t);
      println("quick_sort result: {}", t);
    }
    {
      vector<size_t> v2{ 1, 5, 8, 16, 32, 32, 59, 100 };
      auto [lo,hi] = three_way_partition(v2,32);
      println(
        "three_way_partition: low: {}; equiv: {}; high: {}", 
        std::ranges::subrange(v2.begin(),lo),
        std::ranges::subrange(lo,hi),
        std::ranges::subrange(hi,v2.end())
      );
    }
  }
}
