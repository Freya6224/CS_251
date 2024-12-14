#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/shape.h"

using namespace std;
using namespace testing;

// Circle

TEST(CircleClass, DefaultConstructor) {
  Circle s;
  EXPECT_THAT(s.getX(), Eq(0))
      << "getX on Circle constructed with no args is not 0";
  EXPECT_THAT(s.getY(), Eq(0))
      << "getY on Circle constructed with no args is not 0";
  EXPECT_THAT(s.getRadius(), Eq(0))
      << "getRadius on Circle constructed with no args is not 0";
}

TEST(CircleClass, OneParamConstructor) {
  Circle d(4);
  EXPECT_THAT(d.getX(), Eq(0))
      << "getX on Circle constructed with one arg is wrong";
  EXPECT_THAT(d.getY(), Eq(0))
      << "getY on Circle constructed with one arg is wrong";
  EXPECT_THAT(d.getRadius(), Eq(4))
      << "getRadius on Circle constructed with one arg is wrong";

  Circle e(-5);
  EXPECT_THAT(e.getX(), Eq(0))
      << "getX on Circle constructed with one arg is wrong";
  EXPECT_THAT(e.getY(), Eq(0))
      << "getY on Circle constructed with one arg is wrong";
  EXPECT_THAT(e.getRadius(), Eq(-5))
      << "getRadius on Circle constructed with one arg is wrong";
}

TEST(CircleClass, ThreeParamConstructor) {
  Circle g(321, 123, 32);
  EXPECT_THAT(g.getX(), Eq(321))
      << "getX on Circle constructed with three args is wrong";
  EXPECT_THAT(g.getY(), Eq(123))
      << "getY on Circle constructed with three args is wrong";
  EXPECT_THAT(g.getRadius(), Eq(32))
      << "getRadius on Circle constructed with three args is wrong";

  Circle h(-5, -321, 1);
  EXPECT_THAT(h.getX(), Eq(-5))
      << "getX on Circle constructed with three args is wrong";
  EXPECT_THAT(h.getY(), Eq(-321))
      << "getY on Circle constructed with three args is wrong";
  EXPECT_THAT(h.getRadius(), Eq(1))
      << "getRadius on Circle constructed with three args is wrong";

  Circle j(456, -789, 23);
  EXPECT_THAT(j.getX(), Eq(456))
      << "getX on Circle constructed with three args is wrong";
  EXPECT_THAT(j.getY(), Eq(-789))
      << "getY on Circle constructed with three args is wrong";
  EXPECT_THAT(j.getRadius(), Eq(23))
      << "getRadius on Circle constructed with three args is wrong";
}

TEST(CircleClass, setters) {
  Circle* s = new Circle;
  for (int i = -100; i <= 100; i++) {
    s->setX(i);
    s->setY(i * 2);
    s->setRadius(i * 4);
    ASSERT_THAT(s->getX(), Eq(i)) << "getX after setX returns wrong value";
    ASSERT_THAT(s->getY(), Eq(i * 2)) << "getY after setY returns wrong value";
    ASSERT_THAT(s->getRadius(), Eq(i * 4))
        << "getRadius after setRadius returns wrong value";
  }
  delete s;
}

TEST(CircleClass, copy) {
  Circle* circle = new Circle(4, 3, 5);
  Circle* copy = circle->copy();

  ASSERT_THAT(circle, Ne(copy))
      << "Circle::copy does not return new allocation";
  EXPECT_THAT(copy->getX(), Eq(circle->getX()))
      << "copy has different x from original";
  EXPECT_THAT(copy->getY(), Eq(circle->getY()))
      << "copy has different y from original";
  EXPECT_THAT(copy->getRadius(), Eq(circle->getRadius()))
      << "copy has different radius from original";

  copy->setX(10);
  copy->setY(5);
  copy->setRadius(7);
  EXPECT_THAT(circle->getX(), Eq(4)) << "copy->setX altered original's x";
  EXPECT_THAT(circle->getY(), Eq(3)) << "copy->setY altered original's y";
  EXPECT_THAT(circle->getRadius(), Eq(5))
      << "copy->setRadius altered original's radius";

  delete circle;
  delete copy;
}

TEST(CircleClass, AsString) {
  for (int i = -100; i <= 100; i++) {
    Shape* s = new Circle(i, i + 3, abs(i - 1));
    string expected = "It's a Circle at x: " + to_string(i) + ", " +
                      "y: " + to_string(i + 3) +
                      ", radius: " + to_string(abs(i - 1));
    ASSERT_THAT(s->as_string(), Eq(expected));
    delete s;
  }
}

// Rect

TEST(RectClass, DefaultConstructor) {
  Rect s;
  EXPECT_THAT(s.getX(), Eq(0))
      << "getX on Rect constructed with no args is not 0";
  EXPECT_THAT(s.getY(), Eq(0))
      << "getY on Rect constructed with no args is not 0";
  EXPECT_THAT(s.getWidth(), Eq(0))
      << "getWidth on Rect constructed with no args is not 0";
  EXPECT_THAT(s.getHeight(), Eq(0))
      << "getHeight on Rect constructed with no args is not 0";
}

TEST(RectClass, TwoParamConstructor) {
  Rect d(4, 10);
  EXPECT_THAT(d.getX(), Eq(0))
      << "getX on Rect constructed with two args is not 0";
  EXPECT_THAT(d.getY(), Eq(0))
      << "getY on Rect constructed with two args is not 0";
  EXPECT_THAT(d.getWidth(), Eq(4))
      << "getWidth on Rect constructed with two args is wrong";
  EXPECT_THAT(d.getHeight(), Eq(10))
      << "getHeight on Rect constructed with two args is wrong";
}

TEST(RectClass, FourParamConstructor) {
  Rect h(-5, -321, 2, 15);
  EXPECT_THAT(h.getX(), Eq(-5))
      << "getX on Rect constructed with four args is wrong";
  EXPECT_THAT(h.getY(), Eq(-321))
      << "getY on Rect constructed with four args is wrong";
  EXPECT_THAT(h.getWidth(), Eq(2))
      << "getWidth on Rect constructed with four args is wrong";
  EXPECT_THAT(h.getHeight(), Eq(15))
      << "getHeight on Rect constructed with four args is wrong";
}

TEST(RectClass, setters) {
  Rect* s = new Rect;
  for (int i = -100; i <= 100; i++) {
    s->setX(i);
    s->setY(i * 2);
    s->setWidth(i * 3);
    s->setHeight(i * 4);
    ASSERT_THAT(s->getX(), Eq(i)) << "getX after setX returns wrong value";
    ASSERT_THAT(s->getY(), Eq(i * 2)) << "getY after setY returns wrong value";
    ASSERT_THAT(s->getWidth(), Eq(i * 3))
        << "getWidth after setWidth returns wrong value";
    ASSERT_THAT(s->getHeight(), Eq(i * 4))
        << "getHeight after setHeight returns wrong value";
  }

  delete s;
}

TEST(RectClass, copy) {
  Rect* rect = new Rect(4, 3, 5, 6);
  Rect* copy = rect->copy();

  ASSERT_THAT(rect, Ne(copy))
      << "Rectangle::copy does not return new allocation";
  ASSERT_THAT(copy, Ne(nullptr)) << "Rectangle::copy returns nullptr";
  EXPECT_THAT(copy->getX(), Eq(rect->getX()))
      << "copy has different x from original";
  EXPECT_THAT(copy->getY(), Eq(rect->getY()))
      << "copy has different y from original";
  EXPECT_THAT(copy->getWidth(), Eq(rect->getWidth()))
      << "copy has different width from original";
  EXPECT_THAT(copy->getHeight(), Eq(rect->getHeight()))
      << "copy has different height from original";

  copy->setX(10);
  copy->setY(5);
  copy->setWidth(7);
  copy->setHeight(8);
  EXPECT_THAT(rect->getX(), Eq(4)) << "copy->setX altered original's x";
  EXPECT_THAT(rect->getY(), Eq(3)) << "copy->setY altered original's y";
  EXPECT_THAT(rect->getWidth(), Eq(5))
      << "copy->setWidth altered original's width";
  EXPECT_THAT(rect->getHeight(), Eq(6))
      << "copy->setHeight altered original's height";

  delete rect;
  delete copy;
}

TEST(RectClass, AsString) {
  for (int i = -100; i <= 100; i++) {
    Shape* s = new Rect(i - 2, i + 2, abs(i), abs(i - 5));
    string expected = "It's a Rectangle at x: " + to_string(i - 2) + ", " +
                      "y: " + to_string(i + 2) +
                      " with width: " + to_string(abs(i)) +
                      " and height: " + to_string(abs(i - 5));
    ASSERT_THAT(s->as_string(), Eq(expected));
    delete s;
  }
}

// RightTriangle

TEST(RightTriangleClass, DefaultConstructor) {
  RightTriangle s;
  EXPECT_THAT(s.getX(), Eq(0))
      << "getX on RightTriangle constructed with no args is not 0";
  EXPECT_THAT(s.getY(), Eq(0))
      << "getY on RightTriangle constructed with no args is not 0";
  EXPECT_THAT(s.getBase(), Eq(0))
      << "getBase on RightTriangle constructed with no args is not 0";
  EXPECT_THAT(s.getHeight(), Eq(0))
      << "getHeight on RightTriangle constructed with no args is not 0";
}

TEST(RightTriangleClass, TwoParamConstructor) {
  RightTriangle d(4, 10);
  EXPECT_THAT(d.getX(), Eq(0))
      << "getX on RightTriangle constructed with two args is not 0";
  EXPECT_THAT(d.getY(), Eq(0))
      << "getY on RightTriangle constructed with two args is not 0";
  EXPECT_THAT(d.getBase(), Eq(4))
      << "getWidth on RightTriangle constructed with two args is wrong";
  EXPECT_THAT(d.getHeight(), Eq(10))
      << "getHeight on RightTriangle constructed with two args is wrong";
}

TEST(RightTriangleClass, FourParamConstructor) {
  RightTriangle h(-5, -321, 2, 15);
  EXPECT_THAT(h.getX(), Eq(-5))
      << "getX on RightTriangle constructed with four args is wrong";
  EXPECT_THAT(h.getY(), Eq(-321))
      << "getY on RightTriangle constructed with four args is wrong";
  EXPECT_THAT(h.getBase(), Eq(2))
      << "getBase on RightTriangle constructed with four args is wrong";
  EXPECT_THAT(h.getHeight(), Eq(15))
      << "getHeight on RightTriangle constructed with four args is wrong";
}

TEST(RightTriangleClass, setters) {
  RightTriangle* s = new RightTriangle;
  for (int i = -100; i <= 100; i++) {
    s->setX(i);
    s->setY(i * 2);
    s->setBase(i * 3);
    s->setHeight(i * 4);
    ASSERT_THAT(s->getX(), Eq(i)) << "getX after setX returns wrong value";
    ASSERT_THAT(s->getY(), Eq(i * 2)) << "getY after setY returns wrong value";
    ASSERT_THAT(s->getBase(), Eq(i * 3))
        << "getBase after setWidth returns wrong value";
    ASSERT_THAT(s->getHeight(), Eq(i * 4))
        << "getHeight after setHeight returns wrong value";
  }
  delete s;
}

TEST(RightTriangleClass, copy) {
  RightTriangle* rt = new RightTriangle(4, 3, 5, 6);
  RightTriangle* copy = rt->copy();

  ASSERT_THAT(rt, Ne(copy))
      << "RightTriangle::copy does not return new allocation";
  ASSERT_THAT(copy, Ne(nullptr)) << "RightTriangle::copy returns nullptr";
  EXPECT_THAT(copy->getX(), Eq(rt->getX()))
      << "copy has different x from original";
  EXPECT_THAT(copy->getY(), Eq(rt->getY()))
      << "copy has different y from original";
  EXPECT_THAT(copy->getBase(), Eq(rt->getBase()))
      << "copy has different base from original";
  EXPECT_THAT(copy->getHeight(), Eq(rt->getHeight()))
      << "copy has different height from original";

  copy->setX(10);
  copy->setY(5);
  copy->setBase(7);
  copy->setHeight(8);
  EXPECT_THAT(rt->getX(), Eq(4)) << "copy->setX altered original's x";
  EXPECT_THAT(rt->getY(), Eq(3)) << "copy->setY altered original's y";
  EXPECT_THAT(rt->getBase(), Eq(5)) << "copy->setBase altered original's base";
  EXPECT_THAT(rt->getHeight(), Eq(6))
      << "copy->setHeight altered original's height";

  delete rt;
  delete copy;
}

TEST(RightTriangleClass, AsString) {
  for (int i = -100; i <= 100; i++) {
    Shape* s = new RightTriangle(i - 5, i + 5, abs(i + 1), abs(i - 1));
    string expected = "It's a Right Triangle at x: " + to_string(i - 5) + ", " +
                      "y: " + to_string(i + 5) +
                      " with base: " + to_string(abs(i + 1)) +
                      " and height: " + to_string(abs(i - 1));
    ASSERT_THAT(s->as_string(), Eq(expected));
    delete s;
  }
}
