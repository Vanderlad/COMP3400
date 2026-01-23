constexpr unsigned long long fib(unsigned long long n) noexcept
{
  if (n == 0) [[unlikely]]
    return 0;
  else if (n == 1) [[unlikely]]
    return 1;
  else [[likely]]
    return fib(n-1) + fib(n-2);
}

constexpr unsigned long long factorial(unsigned long long n) noexcept
{
  unsigned long long retval{1};
  for (; n > 1; --n) [[likely]]
    retval *= n;
  return retval;
}
