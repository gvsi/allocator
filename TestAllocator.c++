// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2016
// Giovanni Alcantara
// -------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A> struct TestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       my_allocator<int, 100>,
                       my_allocator<double, 100>> my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator1, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
  const my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

// --------------
// TestAllocator3
// --------------

template <typename A> struct TestAllocator3 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator3, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// Test valid()
// --------------

// Test validity after construction
TEST(TestAllocator4, test_1) {
  my_allocator<int, 200> x;
  ASSERT_EQ(x.valid(), true);
}

// Test validity after allocation
TEST(TestAllocator4, test_2) {
  my_allocator<int, 200> x;
  x.allocate(10);
  ASSERT_EQ(x.valid(), true);
}

// Test validity after deallocation
TEST(TestAllocator4, test_3) {
  my_allocator<int, 200> x;
  int *p = x.allocate(10);
  x.deallocate(p, 10);
  ASSERT_EQ(x.valid(), true);
}

// --------------
// TestAllocator5
// --------------

template <typename A> struct TestAllocator5 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 200>, my_allocator<double, 200>>
    my_types_5;

TYPED_TEST_CASE(TestAllocator5, my_types_5);

// --------------
// Test Allocate()
// --------------

// Check that bad_alloc is thrown when not enough space
TYPED_TEST(TestAllocator5, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 10;

  try {
    const pointer p = x.allocate(s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

// Check that bad_alloc is thrown when N is invalid
TYPED_TEST(TestAllocator5, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 0;
  const value_type v = 10;

  try {
    pointer p = x.allocate(s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

// Check two consecutive allocations
TYPED_TEST(TestAllocator5, test_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 10;

  pointer p = x.allocate(s);
  pointer q = x.allocate(s);
}

// --------------
// Test deallocate()
// --------------

// Check single deallocate
TYPED_TEST(TestAllocator5, test_4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 5;

  pointer p = x.allocate(s);
  x.deallocate(p, v);
}

// Check deallocate with single coalesce
TYPED_TEST(TestAllocator5, test_5) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 5;

  pointer p1 = x.allocate(s);
  pointer p2 = x.allocate(s);
  x.deallocate(p1, v);
  x.deallocate(p2, v);
}

// Check deallocate with two-sided coalesce
TYPED_TEST(TestAllocator5, test_6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 5;
  const value_type v = 5;

  pointer left = x.allocate(s);
  pointer p = x.allocate(s);
  pointer right = x.allocate(s);
  x.deallocate(left, v);
  x.deallocate(right, v);
  x.deallocate(p, v);
}

// Check that exception is thrown when passing nullptr
TYPED_TEST(TestAllocator5, test_7) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 5;
  pointer p = nullptr;

  try {
    x.deallocate(p, s);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

// --------------
// Test Constructor
// --------------

// Check that bad_alloc is thrown when N is less than sizeof(T) + (2 *
// sizeof(int))
TEST(TestAllocator6, test_1) {
  const std::size_t s = 4;
  try {
    const my_allocator<int, s> x;
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

// Check that allocator can be defined as constant
TEST(TestAllocator6, test_2) {
  const std::size_t s = 20;
  my_allocator<int, s> x;
}

// Check default allocation
TEST(TestAllocator6, test_3) {
  const std::size_t s = 20;
  my_allocator<int, s> x;
}
