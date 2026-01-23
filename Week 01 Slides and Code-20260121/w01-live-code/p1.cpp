#include <algorithm>
#include <cmath>
#include <cstddef>
#include <concepts>
#include <format>
#include <generator>
#include <iostream>
#include <iterator>
#include <limits>
#include <numbers>
#include <print>
#include <string>

std::generator<double> first_form()
{
  using std::sqrt;

  double value = sqrt(3.0);
  co_yield value;

  for (;;)
  {
    value = (sqrt(value*value + 1.0) - 1.0) / value;
    co_yield  value;
  }
}

std::generator<double> second_form()
{
  using std::sqrt;

  double value = 1.0 / sqrt(3.0);
  co_yield value;

  for (;;)
  {
    value = value / (sqrt(value*value+1.0)+1.0);
    co_yield value;
  }
}

double form_to_pi(std::size_t i, double value)
{
  return value * std::pow(2.0,i) * 6.0;
}

int main()
{
  using namespace std;

  auto f1 = first_form();
  auto f1it = f1.begin();

  auto f2 = second_form();
  auto f2it = f2.begin();

  for (std::size_t i=0; i != 30; ++i, ++f1it, ++f2it)
  {
    std::cout << i << ' ' << *f1it << ' ' << *f2it << '\n';
  }
  std::cout << '\n';
}

