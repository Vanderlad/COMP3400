# Things Learned

Lots of syntax learned + object lifetime in C++

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
