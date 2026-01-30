# Some things I learned that stood out to me

Lots of syntax learned + object lifetime in CPP + Polymorphism in C++

## Constructors and Assignment

- **Default constructor**  
  → constructs `p_` using its default constructor

- **Value constructor**  
  → constructs `p_` from a provided value

- **Copy constructor**  
  → constructs a new object from an existing one

- **Copy assignment operator**  
  → assigns values into an existing object

- **Move constructor**  
  → constructs a new object by stealing from an expiring one  
  → leaves the source object in a valid but unspecified state

- **Move assignment operator**  
  → replaces an existing object’s contents by stealing from an expiring one  
  → leaves the source object in a valid but unspecified state

## Moved-from Objects

Moved-from objects are:
- valid
- destructible
- but their value is unspecified

## `const` and Right Binding

`const` binds to what is immediately on its left; if nothing is on the left,
it binds to the right.

Declarations are read from the identifier outward (right-to-left), which explains
the term “right binding”.

This is why `shape& const` is illegal: `const` would apply to the reference,
which makes no sense since references cannot be reseated.

### Preferred Style

- Prefer `shape const&`  
  → `const` is placed directly next to what is const  
  → scales better to complex types  
  → common modern C++ stylistic standard


## Destructor behaviour and virtual functions

- Virtual functions dispatch to the most-derived override only; base-class versions are not called unless explicitly invoked.

- A virtual destructor ensures that when an object is deleted through a base-class pointer, the correct derived-class destructor is called, preventing resource leaks and undefined behavior. ex:

```cpp

struct Base {
    virtual ~Base() {}
};

struct Derived : Base {
    ~Derived() {
        // cleanup work
    }
};

Base* b = new Derived;
delete b;

/* 
Runtime Calls:
    Derived::~Derived()
then:
    Base::~Base()
[if virtual wasn't there then it calls Base::~Base() only and Derived::~Derived() is skipped ]
*/
```
## Inheritance and Smart Pointers

*How inheritance syntax works at grammar-level

    The general form is:
    class Derived : access-specifier virtual Base {
    };

- **What does std::make_shared<line>(*this) do?**

    - Allocates memory for a new line object (on the heap)

    - Calls line’s copy constructor

    - Wraps the new object in a std::shared_ptr<line>


- **using oo_shape_type = std::shared_ptr<shape>;**

    - vector<oo_shape_type> v;

    - This is a vector of std::shared_ptr<shape> objects.

    - Object-oriented programming (involving virtual inheritance to use C++ terms) requires using references/pointers to the objects.
    In C++ std::shared_ptr objects automatically free the memory they hold when their destructor is called. The same for std::vector --so your program will have no free/delete calls in it. (This is the norm in C++.)