#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "canvaslist.h"
#include "include/shape.h"

using namespace std;
using namespace testing;

TEST(FakeTest, PleaseDeleteOnceYouWriteSome) {
  // If no tests exist, GoogleTest fails to compile with a fairly confusing
  // message. We have this empty test that does nothing to allow an empty
  // autograder submission to compile and regularly fail. Once you write your
  // own tests for the project, you can delete this one.
  EXPECT_THAT(1, Eq(1));
}
TEST(CanvasListCore, ctor_size) {
  CanvasList canvas = CanvasList();

  ASSERT_THAT(canvas.size(), Eq(0));
  ASSERT_THAT(canvas.empty(), Eq(true));
  ASSERT_THAT(canvas.front(), Eq(nullptr));
}
TEST(CanvasListCore, Size0) {
  CanvasList canvas = CanvasList();

  ASSERT_THAT(canvas.size(), Eq(0));
}

TEST(CanvasListCore, Size1) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  Shape* shape2 = new Shape(2, 3);
  Shape* shape3 = new Shape(4, 3);

  canvas.push_front(shape1);
  ASSERT_THAT(canvas.size(), Eq(1));

  canvas.push_front(shape2);
  ASSERT_THAT(canvas.size(), Eq(2));

  canvas.push_front(shape3);
  ASSERT_THAT(canvas.size(), Eq(3));
}

TEST(CanvasListCore, FrontAlwaysNullPtr) {
  CanvasList canvas = CanvasList();

  ASSERT_THAT(canvas.front(), Eq(nullptr));
  Shape* shape1 = new Shape(2, 3);
  canvas.push_front(shape1);
  ASSERT_THAT(canvas.front()->value, Eq(shape1));
}

TEST(CanvasListCore, PushFrontUpdatesSizeButDoesNothing) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  ASSERT_THAT(canvas.size(), Eq(0));
  canvas.push_front(shape1);
  ASSERT_THAT(canvas.size(), Eq(1));
}
TEST(CanvasListCore, PushFrontDoesNotUpdateSize) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  ASSERT_THAT(canvas.size(), Eq(0));

  canvas.push_front(shape1);
  ASSERT_THAT(canvas.size(), Eq(1));

  Shape* shape2 = new Shape(2, 3);
  canvas.push_front(shape2);
  ASSERT_THAT(canvas.size(), Eq(2));
}

TEST(CanvasListCore, Clear) {
  Shape* shape1 = new Shape(1, 3);
  CanvasList canvas = CanvasList();

  canvas.push_front(shape1);
  canvas.clear();
  ASSERT_THAT(canvas.size(), Eq(0));
  ASSERT_THAT(canvas.empty(), Eq(true));
  ASSERT_THAT(canvas.front(), Eq(nullptr));
}
TEST(CanvasListCore, Destructor) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  canvas.push_front(shape1);
}

TEST(CanvasListIterating, copy_no_use_shape_copy) {
  CanvasList canvas = CanvasList();
  Shape* shape1 = new Rect(4, 6);
  Shape* shape2 = new Circle(4);

  canvas.push_back(shape1);
  canvas.push_back(shape2);

  CanvasList otherCanvas = canvas;

  EXPECT_TRUE(dynamic_cast<Rect*>(otherCanvas.shape_at(0)) != nullptr);
  EXPECT_TRUE(dynamic_cast<Circle*>(otherCanvas.shape_at(1)) != nullptr);
}
TEST(CanvasListIterating, copy_shallow) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(4, 5);
  canvas.push_front(shape1);
  CanvasList otherCanvas(canvas);

  otherCanvas.shape_at(0)->setX(3);
  EXPECT_THAT(shape1->getX(), Eq(4));
}
TEST(CanvasListIterating, copy_empty) {
  CanvasList canvas = CanvasList();
  CanvasList otherCanvas(canvas);
  EXPECT_THAT(otherCanvas.size(), Eq(0));
  EXPECT_THAT(otherCanvas.empty(), Eq(true));
}
TEST(CanvasListIterating, push_back_into_empty) {
  CanvasList canvas = CanvasList();
  Shape* shape1 = new Shape(1, 3);
  ShapeNode* node = new ShapeNode();
  node->value = shape1;
  node->next = nullptr;

  EXPECT_THAT(canvas.size(), Eq(0));

  canvas.push_back(shape1);

  EXPECT_THAT(canvas.size(), Eq(1));
  EXPECT_THAT(canvas.front()->value, Eq(node->value));
  EXPECT_THAT(canvas.front()->next, Eq(node->next));

  delete node;
}
TEST(CanvasListIterating, push_back_nothing) {
  CanvasList canvas = CanvasList();
  EXPECT_THAT(canvas.size(), Eq(0));
}
TEST(CanvasListIterating, push_back_no_update_size) {
  CanvasList canvas = CanvasList();
  Shape* shape1 = new Shape(6, 7);
  Shape* shape2 = new Shape(9, 7);

  ShapeNode* node2 = new ShapeNode();
  node2->value = shape2;
  node2->next = nullptr;

  ShapeNode* node = new ShapeNode();
  node->value = shape1;
  node->next = node2;

  canvas.push_back(shape1);
  canvas.push_back(shape2);

  EXPECT_THAT(canvas.size(), Eq(2));
  EXPECT_THAT(canvas.front()->value, Eq(node->value));
  EXPECT_THAT(canvas.front()->next->value, Eq(node->next->value));
  EXPECT_THAT(canvas.front()->next->value, Eq(node2->value));
  EXPECT_THAT(canvas.front()->next->next, Eq(node2->next));

  delete node;
  delete node2;
}
TEST(CanvasListIterating, shape_at_wrong_index) {
  CanvasList canvas = CanvasList();
  ;
  Shape* shape1 = new Shape(8, 3);
  Shape* shape2 = new Shape(1, 2);

  canvas.push_front(shape1);
  canvas.push_front(shape2);

  EXPECT_THAT(canvas.shape_at(1), Eq(shape1));
  EXPECT_THAT(canvas.shape_at(0), Eq(shape2));
}
TEST(CanvasListIterating, shape_at_upper_bound) {
  CanvasList canvas = CanvasList();
  ;
  Shape* shape1 = new Shape(1, 3);
  canvas.push_front(shape1);

  ASSERT_THAT(canvas.shape_at(1), Eq(nullptr));
}
TEST(CanvasListIterating, find_no_last) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  Shape* shape2 = new Shape(2, 5);
  canvas.push_front(shape1);
  canvas.push_front(shape2);

  EXPECT_THAT(canvas.find(1, 3), Eq(1));
  EXPECT_THAT(canvas.find(2, 5), Eq(0));
}
TEST(CanvasListIterating, find_wrong) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(1, 3);
  Shape* shape2 = new Shape(2, 7);
  canvas.push_front(shape1);
  canvas.push_front(shape2);

  EXPECT_THAT(canvas.find(7, 3), Eq(-1));
  EXPECT_THAT(canvas.find(2, 7), Eq(0));
}
TEST(CanvasListModifying, AssignOperator) {
  CanvasList canvas = CanvasList();
  CanvasList otherCanvas = CanvasList();

  canvas.push_back(new Shape(4, 3));
  canvas.push_back(new Shape(2, 6));
  canvas.push_back(new Shape(9, 7));

  otherCanvas.push_back(new Circle(4, 4, 4));
  otherCanvas.push_back(new Rect(6, 6, 6, 6));
  otherCanvas.push_back(new RightTriangle(2, 2, 2, 2));

  canvas = otherCanvas;

  for (int i = 0; i < 3; i++) {
    EXPECT_THAT(otherCanvas.shape_at(i)->as_string(),
                Eq(canvas.shape_at(i)->as_string()));
  }

  EXPECT_THAT(otherCanvas.size(), Eq(canvas.size()));
  EXPECT_THAT(otherCanvas.front()->value->getX(),
              Eq(canvas.front()->value->getX()));
}
TEST(CanvasListModifying, POP_FRONT) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(2, 5);
  Shape* shape2 = new Shape(3, 7);
  Shape* shape3 = new Shape(1, 8);

  canvas.push_back(shape1);
  canvas.push_back(shape2);
  canvas.push_back(shape3);

  EXPECT_THAT(canvas.pop_front(), Eq(shape1));
  EXPECT_THAT(canvas.front()->value, Eq(shape2));
  EXPECT_THAT(canvas.size(), Eq(2));

  canvas.pop_front();

  EXPECT_THAT(canvas.pop_front(), Eq(shape3));
  EXPECT_THAT(canvas.size(), Eq(0));
  EXPECT_THAT(canvas.front(), Eq(nullptr));

  canvas.pop_front();

  delete shape3;
  delete shape2;
  delete shape1;
}
TEST(CanvasListModifying, POP_BACK) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(2, 5);
  Shape* shape2 = new Shape(3, 7);
  Shape* shape3 = new Shape(1, 8);

  canvas.push_back(shape1);
  canvas.push_back(shape2);
  canvas.push_back(shape3);

  EXPECT_THAT(canvas.pop_back(), Eq(shape3));
  EXPECT_THAT(canvas.size(), Eq(2));

  canvas.pop_back();

  EXPECT_THAT(canvas.pop_back(), Eq(shape1));
  EXPECT_THAT(canvas.size(), Eq(0));
  EXPECT_THAT(canvas.front(), Eq(nullptr));

  canvas.pop_back();

  delete shape3;
  delete shape2;
  delete shape1;
}

TEST(CanvasListModifying, REMOVE_AT) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(2, 5);
  Shape* shape2 = new Shape(3, 7);
  Shape* shape3 = new Shape(1, 8);

  canvas.push_back(shape1);
  canvas.push_back(shape2);
  canvas.push_back(shape3);

  canvas.remove_at(1);
  canvas.remove_at(2);

  EXPECT_THAT(canvas.shape_at(1), Eq(shape3));
  EXPECT_THAT(canvas.size(), Eq(2));

  canvas.remove_at(1);
  canvas.remove_at(0);

  EXPECT_THAT(canvas.shape_at(0), Eq(nullptr));
  EXPECT_THAT(canvas.size(), Eq(0));
}

TEST(CanvasListExtras, Insert_after) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(2, 5);
  Shape* shape2 = new Shape(3, 7);
  Shape* shape3 = new Shape(1, 8);

  canvas.insert_after(0, shape2);

  canvas.push_back(shape1);
  canvas.push_back(shape3);

  canvas.insert_after(0, shape2);
  canvas.insert_after(3, shape2);
  canvas.insert_after(5, shape2);

  EXPECT_THAT(canvas.shape_at(0), Eq(shape1));
  EXPECT_THAT(canvas.shape_at(1), Eq(shape2));
  EXPECT_THAT(canvas.shape_at(2), Eq(shape3));
  EXPECT_THAT(canvas.size(), Eq(3));
}

TEST(CanvasListExtras, Remove_every_other) {
  CanvasList canvas = CanvasList();

  Shape* shape1 = new Shape(2, 5);
  Shape* shape2 = new Shape(3, 7);
  Shape* shape3 = new Shape(1, 8);
  Shape* shape4 = new Shape(1, 8);

  canvas.push_back(shape1);
  canvas.push_back(shape2);
  canvas.push_back(shape3);
  canvas.push_back(shape4);

  canvas.remove_every_other();

  EXPECT_THAT(canvas.shape_at(0), Eq(shape1));
  EXPECT_THAT(canvas.shape_at(1), Eq(shape3));
  EXPECT_THAT(canvas.size(), Eq(2));

  canvas.remove_every_other();

  EXPECT_THAT(canvas.shape_at(0), Eq(shape1));
  EXPECT_THAT(canvas.size(), Eq(1));

  canvas.remove_every_other();
  EXPECT_THAT(canvas.size(), Eq(1));
}