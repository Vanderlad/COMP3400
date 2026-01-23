#include <iostream>
#include <algorithm>
#include <utility>
#include <initializer_list>
class A 
{
  std::size_t size_;
  double *ptr_;

  void size_adjust(A const& b) 
  {
    if (size_ < b.size_)
    { // Extend *this to be as large as b...
      std::cout << "size_adjust() " << this << '\n';
      A tmp{b, b.size_}; swap(tmp);
    } 
    // else *this is same or larger than b...
  }

public:
  void swap(A& b) noexcept 
  { 
    std::swap(ptr_, b.ptr_); 
    std::swap(size_, b.size_); 
  }

  std::size_t size() const noexcept 
  { 
    return size_; 
  }

  double& operator [](std::size_t i) const noexcept 
  {
    return ptr_[i];
  }

  A() : 
    size_{}, 
    ptr_(nullptr) 
  { 
    std::cout << "A() " << this << '\n'; 
  }

  A(std::size_t sz) : 
    size_{sz}, 
    ptr_{new double[sz]} 
  {
    std::cout << "A(" << sz << ") " << this << '\n';
  }

  ~A()
  {
    std::cout << "~A() " << this << '\n';
    delete[] ptr_;
  }

  A(std::initializer_list<double> il) :
    size_{il.size()}, 
    ptr_{new double[il.size()]} 
  {
    std::cout << "A(init_list:" << size_ << ") " << this << '\n';
    std::copy(il.begin(), il.end(), ptr_);
  }

  A(A const& b, std::size_t sz) : 
    size_{std::max(b.size_,sz)},
    ptr_(new double[size_])
  {
    std::cout << "A(copy," << sz << ") " << this << ' ' << &b << '\n';

    std::copy(b.ptr_, b.ptr_+b.size_, ptr_); // copy data
    std::fill_n(ptr_, size_-b.size_, double{}); // zero data
  }

  A(A const& b) : 
    size_{b.size_}, ptr_(new double[b.size_]) 
  {
    std::cout << "A(copy) " << this << ' ' << &b << '\n';
    std::copy(b.ptr_, b.ptr_+b.size_, ptr_); // copy data
  }

  A& operator =(A const& b) {
    std::cout << "a=b;(copy) " << this << ' ' << &b << '\n';
    if (size_ != b.size_) {
      A tmp{b}; swap(tmp); // copy and swap
    } else { 
      size_adjust(b); // adjust size if needed
      std::copy(b.ptr_, b.ptr_+b.size_, ptr_); // copy data
      size_ = b.size_;
    }
    return *this;
  }

  A(A&& b) : 
    size_{std::move(b.size_)}, // move size
    ptr_{std::move(b.ptr_)} // move pointer
  {
    std::cout << "A(move) " << this << ' ' << &b << '\n';

    b.size_ = {}; // zero out original size
    b.ptr_ = nullptr; // null out original pointer
  }

  A& operator =(A&& b) 
  {
    std::cout << "a=b;(move) " << this << ' ' << &b << '\n';
    swap(b); // swap
    return *this;
  }

  A operator +(A const& b) const
  {
    std::cout << "a+b;(const) " << this << ' ' << &b << '\n';

    A retval{*this, std::max(size_, b.size_)}; // copy *this
    for (std::size_t i{}; i != b.size_; ++i)
      retval.ptr_[i] += b.ptr_[i];
    return retval;
  }

  A operator +(A&& b) const {
    std::cout << "a+b;(rvalue1) " << this << ' ' << &b << '\n';
    b.size_adjust(*this); // adjust size if needed

    for (std::size_t i{}; i != size_; ++i)
      b.ptr_[i] += ptr_[i]; // Add *this.ptr to b.ptr!
    return std::move(b); // Move b into return value
  }
};

A operator +(A&& a, A const& b) { 
  std::cout << "a+b;(rvalue2) " << &a << ' ' << &b << '\n';
  return b+std::move(a);
}

A operator +(A&& a, A&& b) { 
  std::cout << "a+b;(rvalue3) " << &a << ' ' << &b << '\n';
  return a+std::move(b);
}

int main() { 
  A result = A(5) + A{0.0, 1.1, 2.2, 3.3, 4.4};
}
