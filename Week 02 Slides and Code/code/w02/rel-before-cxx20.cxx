// for some user-defined type, e.g.,...
struct foo { int i; };
// one needs to write all relevant/needed relational operator overloads which
// must return bool and need const& parameters, e.g.,
bool operator==(foo const& a, foo const& b) { return a.i == b.i; }
bool operator!=(foo const& a, foo const& b) { return a.i != b.i; }
bool operator <(foo const& a, foo const& b) { return a.i < b.i; }
bool operator<=(foo const& a, foo const& b) { return a.i <= b.i; }
bool operator>=(foo const& a, foo const& b) { return a.i >= b.i; }
bool operator >(foo const& a, foo const& b) { return a.i > b.i; }

int main()
{
  foo a{1}, b{2};
  if (a < b) { /* ... */ }
}
