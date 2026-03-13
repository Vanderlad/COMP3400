#ifndef include_concurrent_queue_hpp_
#define include_concurrent_queue_hpp_

#include <compare>
#include <concepts>
#include <deque>
#include <format>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace comp3400_2026w {

template <typename R, typename ValueType>
concept compatible_input_iterator_c =
  std::input_iterator<R> &&
  std::convertible_to<std::iter_value_t<R>, ValueType>;

template <typename R, typename ValueType>
concept compatible_input_range_c =
  std::ranges::input_range<R> &&
  std::convertible_to<std::ranges::range_value_t<R>, ValueType>;

template <typename T, typename Container = std::deque<T>>
class concurrent_queue :
  protected std::queue<T, Container>
{
  template <typename T2, typename Ch>
  friend struct std::formatter;

protected:
  using inherited_queue = std::queue<T, Container>;

public:
  using queue_type = inherited_queue;
  using container_type = Container;
  using value_type = typename queue_type::value_type;
  using size_type = typename queue_type::size_type;

protected:
  mutable std::mutex mutex_;

  queue_type& underlying_queue() noexcept
  {
    return *static_cast<inherited_queue*>(this);
  }

  queue_type const& underlying_queue() const noexcept
  {
    return *static_cast<inherited_queue const*>(this);
  }

public:
  concurrent_queue() = default;

  explicit concurrent_queue(queue_type const& q) :
    inherited_queue{q}
  {
  }

  concurrent_queue(concurrent_queue const& other)
  {
    std::lock_guard lk{other.mutex_};
    inherited_queue::operator=(static_cast<queue_type const&>(other));
  }

  concurrent_queue& operator=(queue_type const& q)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::operator=(q);
    return *this;
  }

  concurrent_queue& operator=(concurrent_queue const& other)
  {
    if (this != std::addressof(other))
    {
      std::scoped_lock lk{mutex_, other.mutex_};
      inherited_queue::operator=(static_cast<queue_type const&>(other));
    }
    return *this;
  }

  explicit concurrent_queue(queue_type&& q) :
    inherited_queue{std::move(q)}
  {
  }

  concurrent_queue(concurrent_queue&& other)
  {
    std::lock_guard lk{other.mutex_};
    inherited_queue::operator=(std::move(other.underlying_queue()));
  }

  concurrent_queue& operator=(queue_type&& q)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::operator=(std::move(q));
    return *this;
  }

  concurrent_queue& operator=(concurrent_queue&& other)
  {
    if (this != std::addressof(other))
    {
      std::scoped_lock lk{mutex_, other.mutex_};
      inherited_queue::operator=(std::move(other.underlying_queue()));
    }
    return *this;
  }

  explicit concurrent_queue(container_type const& c) :
    inherited_queue{c}
  {
  }

  explicit concurrent_queue(container_type&& c) :
    inherited_queue{std::move(c)}
  {
  }

  template <typename InputIt>
  requires compatible_input_iterator_c<InputIt, value_type>
  concurrent_queue(InputIt first, InputIt last) :
    inherited_queue{first, last}
  {
  }

  template <typename R>
  requires compatible_input_range_c<R, value_type>
  concurrent_queue(std::from_range_t, R&& r) :
    inherited_queue{std::from_range, std::forward<R>(r)}
  {
  }

  void clear()
  {
    queue_type tmp;
    std::lock_guard lk{mutex_};
    tmp.swap(underlying_queue());
  }

  std::optional<value_type> front() const
  {
    std::lock_guard lk{mutex_};
    if (!inherited_queue::empty())
      return inherited_queue::front();
    return std::nullopt;
  }

  std::optional<value_type> back() const
  {
    std::lock_guard lk{mutex_};
    if (!inherited_queue::empty())
      return inherited_queue::back();
    return std::nullopt;
  }

  bool empty() const
  {
    std::lock_guard lk{mutex_};
    return inherited_queue::empty();
  }

  size_type size() const
  {
    std::lock_guard lk{mutex_};
    return inherited_queue::size();
  }

  void push(value_type const& v)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::push(v);
  }

  void push(value_type&& v)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::push(std::move(v));
  }

  bool try_push(value_type const& v)
  {
    std::unique_lock lk{mutex_, std::try_to_lock};
    if (!lk)
      return false;
    inherited_queue::push(v);
    return true;
  }

  bool try_push(value_type&& v)
  {
    std::unique_lock lk{mutex_, std::try_to_lock};
    if (!lk)
      return false;
    inherited_queue::push(std::move(v));
    return true;
  }

  template <typename R>
  requires compatible_input_range_c<R, value_type>
  void push_range(R&& r)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::push_range(std::forward<R>(r));
  }

  template <typename R>
  requires compatible_input_range_c<R, value_type>
  bool try_push_range(R&& r)
  {
    std::unique_lock lk{mutex_, std::try_to_lock};
    if (!lk)
      return false;
    inherited_queue::push_range(std::forward<R>(r));
    return true;
  }

  template <typename... Args>
  void emplace(Args&&... args)
  {
    std::lock_guard lk{mutex_};
    inherited_queue::emplace(std::forward<Args>(args)...);
  }

  template <typename... Args>
  bool try_emplace(Args&&... args)
  {
    std::unique_lock lk{mutex_, std::try_to_lock};
    if (!lk)
      return false;
    inherited_queue::emplace(std::forward<Args>(args)...);
    return true;
  }

  std::optional<value_type> pop()
  {
    std::lock_guard lk{mutex_};
    if (inherited_queue::empty())
      return std::nullopt;

    std::optional<value_type> retval{std::move(inherited_queue::front())};
    inherited_queue::pop();
    return retval;
  }

  auto try_pop() -> std::tuple<bool, std::optional<value_type>>
  {
    std::unique_lock lk{mutex_, std::try_to_lock};
    if (!lk)
      return {false, std::nullopt};

    if (inherited_queue::empty())
      return {true, std::nullopt};

    std::tuple<bool, std::optional<value_type>> retval{
      true, std::move(inherited_queue::front())
    };
    inherited_queue::pop();
    return retval;
  }

  bool operator==(concurrent_queue const& other) const
  {
    std::scoped_lock lk{mutex_, other.mutex_};
    return static_cast<queue_type const&>(*this) ==
           static_cast<queue_type const&>(other);
  }

  auto operator<=>(concurrent_queue const& other) const
  {
    std::scoped_lock lk{mutex_, other.mutex_};
    return static_cast<queue_type const&>(*this) <=>
           static_cast<queue_type const&>(other);
  }

  void swap(concurrent_queue& other)
  {
    if (this == std::addressof(other))
      return;

    std::scoped_lock lk{mutex_, other.mutex_};
    underlying_queue().swap(other.underlying_queue());
  }
};

template <typename T, typename Container>
inline void swap(
  concurrent_queue<T, Container>& a,
  concurrent_queue<T, Container>& b
)
{
  a.swap(b);
}

} // namespace comp3400_2026w

namespace std {

template <typename T, typename Container, typename Alloc>
struct uses_allocator<comp3400_2026w::concurrent_queue<T, Container>, Alloc> :
  uses_allocator<Container, Alloc>::type
{
};

template <typename Ch, typename T, typename Container>
requires std::formattable<T, Ch>
struct formatter<comp3400_2026w::concurrent_queue<T, Container>, Ch>
{
  constexpr auto parse(std::basic_format_parse_context<Ch>& ctx)
  {
    auto it = ctx.begin();
    if (it != ctx.end() && *it != '}')
      throw std::format_error("invalid format specifier for concurrent_queue");
    return it;
  }

  template <typename FormatContext>
  auto format(
    comp3400_2026w::concurrent_queue<T, Container> const& cq,
    FormatContext& ctx
  ) const
  {
    std::lock_guard lk{cq.mutex_};

    auto out = ctx.out();
    auto const& cont =
      cq.comp3400_2026w::concurrent_queue<T, Container>::inherited_queue::c;

    if (cont.empty())
      return std::format_to(out, "<empty>");

    bool first = true;
    for (auto const& x : cont)
    {
      if (!first)
        out = std::format_to(out, " ");
      out = std::format_to(out, "{}", x);
      first = false;
    }

    return out;
  }
};

} // namespace std

#endif // include_concurrent_queue_hpp_