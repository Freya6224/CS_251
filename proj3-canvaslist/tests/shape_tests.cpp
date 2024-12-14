#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/shape.h"

using namespace std;
using namespace testing;

TEST(ShapeClass, DefaultConstructor) {
  Shape s;
  EXPECT_THAT(s.getX(), Eq(0))
      << "getX on Shape constructed with no args is not 0";
  EXPECT_THAT(s.getY(), Eq(0))
      << "getY on Shape constructed with no args is not 0";
}

TEST(ShapeClass, ParamConstructor) {
  Shape s(1, 4);
  EXPECT_THAT(s.getX(), Eq(1))
      << "getX on Shape constructed with args is wrong";
  EXPECT_THAT(s.getY(), Eq(4))
      << "getY on Shape constructed with args is wrong";

  // alternate positive x, y generic Shape
  Shape q(300, 150);
  EXPECT_THAT(q.getX(), Eq(300))
      << "getX on Shape constructed with args is wrong";
  EXPECT_THAT(q.getY(), Eq(150))
      << "getY on Shape constructed with args is wrong";

  // negative x, y generic Shape
  Shape b(-450, -34);
  EXPECT_THAT(b.getX(), Eq(-450))
      << "getX on Shape constructed with args is wrong";
  EXPECT_THAT(b.getY(), Eq(-34))
      << "getY on Shape constructed with args is wrong";

  // mixed x, y generic Shape
  Shape c(123, -56);
  EXPECT_THAT(c.getX(), Eq(123))
      << "getX on Shape constructed with args is wrong";
  EXPECT_THAT(c.getY(), Eq(-56))
      << "getY on Shape constructed with args is wrong";

  // mixed x, y generic Shape
  Shape d(-456, 93);
  EXPECT_THAT(d.getX(), Eq(-456))
      << "getX on Shape constructed with args is wrong";
  EXPECT_THAT(d.getY(), Eq(93))
      << "getY on Shape constructed with args is wrong";
}

TEST(ShapeClass, setX) {
  Shape* s = new Shape;
  for (int i = -100; i <= 100; i++) {
    s->setX(i);
    ASSERT_THAT(s->getX(), Eq(i)) << "getX after setX returns wrong value";
    ASSERT_THAT(s->getY(), Eq(0)) << "getY after setX returns different value";
  }

  delete s;
}

TEST(ShapeClass, setY) {
  Shape* s = new Shape;
  for (int i = -100; i <= 100; i++) {
    s->setY(i);
    ASSERT_THAT(s->getY(), Eq(i)) << "getY after setY returns wrong value";
    ASSERT_THAT(s->getX(), Eq(0)) << "getX after setY returns different value";
  }

  delete s;
}

TEST(ShapeClass, copy) {
  Shape* shape = new Shape(4, 3);
  Shape* copy = shape->copy();

  ASSERT_THAT(shape, Ne(copy)) << "Shape::copy does not return new allocation";
  EXPECT_THAT(shape->getX(), Eq(copy->getX()))
      << "copy has different x from original";
  EXPECT_THAT(shape->getY(), Eq(copy->getY()))
      << "copy has different y from original";

  copy->setX(10);
  copy->setY(5);
  EXPECT_THAT(shape->getX(), Eq(4)) << "copy->setX altered original's x";
  EXPECT_THAT(shape->getY(), Eq(3)) << "copy->setY altered original's y";
  EXPECT_THAT(copy->getX(), Eq(10)) << "getX after setX returns wrong value";
  EXPECT_THAT(copy->getY(), Eq(5)) << "getY after setY returns wrong value";

  // Clean up memory!
  delete shape;
  delete copy;
}

TEST(ShapeClass, AsString) {
  for (int i = -100; i <= 100; i++) {
    Shape* s = new Shape(i + 1, i);
    string expected =
        "It's a Shape at x: " + to_string(i + 1) + ", " + "y: " + to_string(i);
    ASSERT_THAT(s->as_string(), Eq(expected))
        << "Built with parameterized constructor";
    delete s;
  }
}
