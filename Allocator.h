// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2016
// Giovanni Alcantara
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

  // -----
  // valid
  // -----

  /**
  * O(1) in space
  * O(n) in time
  * <your documentation>
  */
  bool valid() const {
    const char *first_sent_ptr = a;
    const int *first_sent_int = reinterpret_cast<const int*>(first_sent_ptr);
    const char *second_sent_ptr = first_sent_ptr + abs(*first_sent_int) + 4;

    bool block_is_free = false;
    while (first_sent_ptr < a + N - 4) {
      const int *p = reinterpret_cast<const int*>(first_sent_ptr);
      const int *q = reinterpret_cast<const int*>(second_sent_ptr);

      if (*p == 0) { // invalid empty block
        return false;
      }

      if (*p > 0) {
        if (block_is_free) { // Found two adjacent blocks
          return false;
        }
        block_is_free = true;
      } else {
        block_is_free = false;
      }



      if (*p != *q) {
        // std::cout << "NO! " << *p << "!=" << *q << std::endl;
        return false;
      } else {
        // std::cout << "YES! " << *p << "==" << *q << std::endl;
      }

      // increment sentinels
      first_sent_ptr = second_sent_ptr + 4;
      second_sent_ptr = first_sent_ptr + abs(*(int *)(first_sent_ptr)) + 4;
    }
    //std::cout << "VALID!" << std::endl;
    return true;
  }

  /**
  * O(1) in space
  * O(1) in time
  * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
  */
  FRIEND_TEST(TestAllocator2, index);
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

    //print_array();

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
    int min_block_size = sizeof(value_type) + (2 * sizeof(int)); // 16
    int alloc_req_bytes = n * sizeof(value_type);                // 24
    int min_req_bytes = n * sizeof(value_type) + min_block_size; // 40

    char *first_sent_ptr = a;
    int *first_sent_int = reinterpret_cast<int*>(first_sent_ptr);
    char *second_sent_ptr = first_sent_ptr + abs(*first_sent_int) + 4;

    // Check that the values in the two sentinels are the same
    assert(*reinterpret_cast<int*>(first_sent_ptr) == *reinterpret_cast<int*>(second_sent_ptr));

    while (first_sent_ptr < a + N - 4) {
      int *p = reinterpret_cast<int*>(first_sent_ptr);
      int *q = reinterpret_cast<int*>(second_sent_ptr);
      assert(*p == *q);

      if (*p == alloc_req_bytes) { // user gets exactly what was requested
        *p = *p * -1;
        *q = *q * -1;

        assert(valid());
        return reinterpret_cast<pointer>(p + 1);
      } else if (*p > alloc_req_bytes &&
                 *p < min_req_bytes) { // user gets more than what was requested

        *p = *p * -1;
        *q = *q * -1;

        assert(valid());
        return reinterpret_cast<pointer>(p + 1);
      } else if (*p >= min_req_bytes) { // user gets exactly what was requested

        int remaining_free_bytes =
            *p - alloc_req_bytes - 8; // the bytes that will be free on the
                                      // adjacent block on the right (value
                                      // will be displayed on the new sentinels)
        *p = alloc_req_bytes * -1;
        *reinterpret_cast<int *>(first_sent_ptr + 4 + alloc_req_bytes) =
            alloc_req_bytes * -1; // sets new sentinel

        *reinterpret_cast<int *>(first_sent_ptr + 4 + alloc_req_bytes + 4) = remaining_free_bytes;
        *q = remaining_free_bytes;

        assert(valid());
        return reinterpret_cast<pointer>(p + 1);
      }

      // increment sentinels
      first_sent_ptr = second_sent_ptr + 4;
      second_sent_ptr = first_sent_ptr + abs(*(int *)(first_sent_ptr)) + 4;
    }

    std::bad_alloc exception;
    throw exception;
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
    if (p == nullptr) {
      std::invalid_argument exception("Invalid pointer");
      throw exception;
    }

    assert(valid());

    int* first_sent_ptr = reinterpret_cast<int*>(p) - 1;
    char* first_sent_char_ptr = reinterpret_cast<char*>(first_sent_ptr);
    int* second_sent_ptr = reinterpret_cast<int*>(first_sent_char_ptr + abs(*first_sent_ptr) + 4);

    if (*first_sent_ptr != *second_sent_ptr) {
      std::cout << *first_sent_ptr << " " << *second_sent_ptr << std::endl;
    }
    assert(*first_sent_ptr == *second_sent_ptr);

    // Check whether there are adjancent blocks
    bool is_first_block = first_sent_ptr == reinterpret_cast<int*>(a);
    bool is_last_block = second_sent_ptr == reinterpret_cast<int*>(a+N) - 1;

    // Keeps track of all the bytes that we free after potential coalescing
    int freed_bytes = abs(*first_sent_ptr);

    if (!is_first_block) {
      int* prev_ptr = first_sent_ptr - 1;
      if (*prev_ptr > 0) { // we can coalesce with previous block
        // Increment freed bytes counter by size of adjacent free block
        freed_bytes += *prev_ptr + 8;

        // Shift lower bound sentinel to previous block
        first_sent_ptr = reinterpret_cast<int*>(reinterpret_cast<char*>(prev_ptr) - *prev_ptr - 4);
        assert(*first_sent_ptr > 0);
        assert(*first_sent_ptr == *prev_ptr);
      }
    }

    if (!is_last_block) {
      int* next_ptr = second_sent_ptr + 1;
      if (*next_ptr > 0) { // we can coalesce with next block
        // Increment freed bytes counter by size of adjacent free block
        freed_bytes += *next_ptr + 8;

        // Shift upper bound sentinel to next block
        second_sent_ptr = reinterpret_cast<int*>(reinterpret_cast<char*>(next_ptr) + *next_ptr + 4);
        assert(*second_sent_ptr > 0);
        assert(*second_sent_ptr == *next_ptr);
      }
    }

    // Set the values of the resized sentinels
    *first_sent_ptr = freed_bytes;
    *second_sent_ptr = freed_bytes;

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

  void print_array() {
    for (int i = 0; i < N; ++i) {
      std::cout << i << ": " << (*this)[i] << " " << std::endl;
    }
    std::cout << std::endl << std::endl;
  }

  const int &operator[](int i) const {
    return *reinterpret_cast<const int *>(&a[i]);
  }
};

#endif // Allocator_h
