// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2016
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <iostream>
#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument

// ---------
// Allocator
// ---------
template <typename T, std::size_t N> class my_allocator {
public:
  // --------
  // typedefs
  // --------

  typedef T value_type;

  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  typedef value_type &reference;
  typedef const value_type &const_reference;

public:
  // -----------
  // operator ==
  // -----------

  friend bool operator==(const my_allocator &, const my_allocator &) {
    return false;
  } // this is correct

  // -----------
  // operator !=
  // -----------

  friend bool operator!=(const my_allocator &lhs, const my_allocator &rhs) {
    return !(lhs == rhs);
  }

private:
  // ----
  // data
  // ----

  char a[N];

  void print_array() {
    for (int i = 0; i < N; ++i) {
      std::cout << i << ": " << (*this)[i] << " " << std::endl;
    }
  }
  // -----
  // valid
  // -----

  /**
  * O(1) in space
  * O(n) in time
  * <your documentation>
  */
  bool valid() const {
    const char *first_sent_ptr = &a[0];
    const char *second_sent_ptr = &a[abs(a[0]) + 4];

    while (first_sent_ptr < a + N) {
      const int *p = (int *)(first_sent_ptr);
      const int *q = (int *)(second_sent_ptr);
      if (*p != *q) {
        std::cout << "NO! " << *p << "!=" << *q << std::endl;
        return false;
      } else {
        std::cout << "YES! " << *p << "==" << *q << std::endl;
      }

      first_sent_ptr = second_sent_ptr + 4;
      second_sent_ptr = first_sent_ptr + abs(*(int *)(first_sent_ptr)) + 4;
    }

    return true;
  }

  /**
  * O(1) in space
  * O(1) in time
  * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
  */
  // FRIEND_TEST(TestAllocator2, index);
  int &operator[](int i) { return *reinterpret_cast<int *>(&a[i]); }

public:
  // ------------
  // constructors
  // ------------

  /**
  * O(1) in space
  * O(1) in time
  * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
  */
  my_allocator() {
    if (N < sizeof(T) + (2 * sizeof(int))) {
      std::bad_alloc exception;
      throw exception;
    }

    (*this)[0] = N - 8;
    (*this)[N - 4] = N - 8;

    print_array();

    assert(valid());
  }

  my_allocator(const my_allocator &) = default;
  ~my_allocator() = default;
  my_allocator &operator=(const my_allocator &) = default;

  // --------
  // allocate
  // --------

  /**
  * O(1) in space
  * O(n) in time
  * after allocation there must be enough space left for a valid block
  * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
  * choose the first block that fits
  * throw a bad_alloc exception, if n is invalid
  */
  pointer allocate(size_type n) {
    

    return 0;
    return nullptr;
  }

  // ---------
  // construct
  // ---------

  /**
  * O(1) in space
  * O(1) in time
  */
  void construct(pointer p, const_reference v) {
    new (p) T(v); // this is correct and exempt
    assert(valid());
  } // from the prohibition of new

  // ----------
  // deallocate
  // ----------

  /**
  * O(1) in space
  * O(1) in time
  * after deallocation adjacent free blocks must be coalesced
  * throw an invalid_argument exception, if p is invalid
  * <your documentation>
  */
  void deallocate(pointer p, size_type) {
    // <your code>
    assert(valid());
  }

  // -------
  // destroy
  // -------

  /**
  * O(1) in space
  * O(1) in time
  */
  void destroy(pointer p) {
    p->~T(); // this is correct
    assert(valid());
  }

  /**
  * O(1) in space
  * O(1) in time
  */

  const int &get(int i) const { return *reinterpret_cast<const int *>(&a[i]); }

  const int &operator[](int i) const {
    return *reinterpret_cast<const int *>(&a[i]);
  }
};

#endif // Allocator_h
