#ifndef include_concurrent_queue_concepts_hpp_
#define include_concurrent_queue_concepts_hpp_

//============================================================================

#include <concepts>
#include <format>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>

//=============================================================================

namespace comp3400_2026w {

//=============================================================================

template <typename T>
concept concurrent_queue_c =
  requires (T ct, T t)
  {
    typename T::value_type;
    typename T::size_type;

    requires std::unsigned_integral<typename T::size_type>;
  } &&
  std::default_initializable<T> &&
  std::destructible<T> &&

  // copy-related...
  std::constructible_from<T,T const&> && // i.e., copy construction
  std::assignable_from<T&,T const&> && // i.e., copy assignment

  // move-related...
  std::constructible_from<T,T&&> && // i.e., move construction
  std::assignable_from<T&,T&&> && // i.e., move assignment

  // base-type related...
  std::is_base_of_v<typename T::queue_type,T> &&
  // NOTE: std::derived_from<T,typename T::queue_type> cannot be used
  //       since the derivation is not public so <type_traits>'s 
  //       std::is_base_of is used to perform this requirement check.
  //       (It is typically best to inherit non-publicly since such will 
  //       cause the base class' public members to not be public in the 
  //       derived class.)
  std::constructible_from<T,typename T::queue_type> &&
  std::is_assignable_v<T&,typename T::queue_type const&> &&
  std::is_assignable_v<T&,typename T::queue_type&&> &&
  // NOTE: std::assignable_from concept cannot be used since there is no common
  //       reference between T& and T::queue_type const& (when the 
  //       inheritance of queue_type is not public) so
  //       <type_traits>'s std::is_assignable is used to perform this
  //       requirement check.

  std::constructible_from<T,typename T::container_type const&> &&
  std::constructible_from<T,typename T::container_type&&> &&

  requires (
    T t, T const ct,
    typename T::value_type const& cv, typename T::value_type&& rv,
    typename T::container_type const& cc, typename T::container_type&& rc
  )
  {
    { t.clear() } -> std::same_as<void>;

    { ct.front() } -> std::same_as<std::optional<typename T::value_type>>;
    { ct.back() } -> std::same_as<std::optional<typename T::value_type>>;
    { ct.empty() } -> std::same_as<bool>;
    { ct.size() } -> std::same_as<typename T::size_type>;

    { t.push(cv) } -> std::same_as<void>;
    { t.push(rv) } -> std::same_as<void>;
    { t.try_push(cv) } -> std::same_as<bool>;
    { t.try_push(rv) } -> std::same_as<bool>;

    { t.pop() }
      -> std::same_as<std::optional<typename T::value_type>>;
    { t.try_pop() }
      -> std::same_as<std::tuple<bool,std::optional<typename T::value_type>>>;

    // NOTE: Since we know the container type one can test pushing
    //       that as a range --but one cannot test other ranges given
    //       the template parameters this concept has. This
    //       implicitly requires the container type to also be a range.
    //       (A check is probably better than none at all.)
    { t.push_range(cc) } -> std::same_as<void>;
    { t.push_range(rc) } -> std::same_as<void>;
    { t.try_push_range(cc) } -> std::same_as<bool>;
    { t.try_push_range(rc) } -> std::same_as<bool>;

    // NOTE: Since we know the container type one can test emplacing
    //       that as a range --but one cannot test other inputs given
    //       the template parameters this concept has and does not have.
    //       (A check is probably better than none at all.)
    { t.emplace(cc) } -> std::same_as<void>;
    { t.emplace(rc) } -> std::same_as<void>;
    { t.try_emplace(cc) } -> std::same_as<bool>;
    { t.try_emplace(rc) } -> std::same_as<bool>;
  } &&
  std::equality_comparable<T> &&
  std::three_way_comparable<T> &&

  // if value_type is swappable then this T should be swappable...
  (
    not std::swappable<typename T::value_type> ||
    std::swappable<T>
  )
;

//=============================================================================

} // namespace comp3400_2026w

//=============================================================================

#endif // #ifndef include_concurrent_queue_concepts_hpp_
