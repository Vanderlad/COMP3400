#include <iostream>
struct [[nodiscard]] super_important { };
struct foo { 
  int i; 
  [[nodiscard]] bool operator==(foo const& f) const { return i == f.i; }
  static super_important bar() { return {}; }
};
int main() {
  foo a{1}, b{2};
  a == b;       // warning issued since bool is discarded
  foo::bar();   // warning issued since super_important is discarded
}
