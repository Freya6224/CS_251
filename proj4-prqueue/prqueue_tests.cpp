// Why the angle brackets? Grader reasons.
// Don't change this or you will be sad :(
#include <prqueue.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace std;
using namespace testing;

TEST(EmptyTest, Test) {
  prqueue<int> pq;
}

TEST(PRQueueCore, DefaultConstructorTest) {
  prqueue<int> queue;
  EXPECT_THAT(queue.size(), Eq(0));
  EXPECT_THAT(queue.getRoot(), Eq(nullptr));
}

TEST(PRQueueCore, SizeTest) {
  prqueue<int> queue;
  queue.enqueue(11, 1);
  queue.enqueue(12, 2);
  EXPECT_THAT(queue.size(), Eq(2));

  queue.clear();
  EXPECT_THAT(queue.size(), Eq(0));
}

TEST(PRQueueCore, GetRootTest) {
  prqueue<int> queue;

  EXPECT_THAT(queue.getRoot(), Eq(nullptr));

  queue.enqueue(11, 1);

  auto root = queue.getRoot();

  ASSERT_NE(root, (nullptr));
  EXPECT_THAT(root->priority, Eq(1));
  EXPECT_THAT(root->value, Eq(11));
}

TEST(PRQueueCore, EnqueueTest) {
  prqueue<int> queue;

  queue.enqueue(10, 5);

  auto root = queue.getRoot();
  ASSERT_NE(root, (nullptr));
  EXPECT_THAT(root->priority, Eq(5));
  EXPECT_THAT(root->value, Eq(10));

  queue.enqueue(20, 3);

  ASSERT_NE(root->left, (nullptr));
  EXPECT_THAT(root->left->value, Eq(20));
  EXPECT_THAT(root->left->priority, Eq(3));

  queue.enqueue(30, 7);

  ASSERT_NE(root->right, (nullptr));
  EXPECT_THAT(root->right->value, Eq(30));
  EXPECT_THAT(root->right->priority, Eq(7));

  EXPECT_THAT(queue.size(), 3);
}
TEST(PRQueueCore, ClearTest) {
  prqueue<int> queue;
  queue.enqueue(11, 1);
  queue.enqueue(12, 2);
  EXPECT_THAT(queue.size(), Eq(2));

  queue.clear();
  EXPECT_THAT(queue.size(), Eq(0));
  EXPECT_THAT(queue.getRoot(), Eq(nullptr));
}

TEST(PRQueueUsing, asStringAllElementsTest) {
  prqueue<int> queue;
  queue.enqueue(1, 10);
  queue.enqueue(2, 8);
  queue.enqueue(3, 17);

  EXPECT_THAT(queue.as_string(), Eq("8 value: 2\n10 value: 1\n17 value: 3\n"));
}

TEST(PRQueueUsing, asStringProperFormattedTest) {
  prqueue<std::string> queue;
  queue.enqueue("Bob", 1);
  queue.enqueue("Alice", 8);
  queue.enqueue("Charlie", 7);

  EXPECT_THAT(queue.as_string(),
              Eq("1 value: Bob\n7 value: Charlie\n8 value: Alice\n"));
}

TEST(PRQueueUsing, peekFrontValTest) {
  prqueue<int> queue;
  queue.enqueue(100, 10);
  queue.enqueue(50, 7);
  queue.enqueue(75, 47);

  EXPECT_THAT(queue.peek(), Eq(50));
}

TEST(PRQueueUsing, peekEmptyQueue) {
  prqueue<int> queue;

  EXPECT_THAT(queue.peek(), Eq(int{}));
}

TEST(PRQueueUsing, dequeueFrontValTest) {
  prqueue<int> queue;
  queue.enqueue(1, 3);
  queue.enqueue(2, 1);
  queue.enqueue(3, 5);

  EXPECT_THAT(queue.dequeue(), Eq(2));
  EXPECT_THAT(queue.size(), Eq(2));
}

TEST(PRQueueUsing, dequeueUpdateSizeTest) {
  prqueue<int> queue;
  queue.enqueue(1, 3);
  queue.enqueue(2, 1);
  queue.dequeue();

  EXPECT_THAT(queue.size(), Eq(1));
}

TEST(PRQueueUsing, dequeueRemovesNodeTest) {
  prqueue<int> queue;
  queue.enqueue(30, 30);
  queue.enqueue(10, 10);
  queue.enqueue(20, 20);
  queue.dequeue();

  EXPECT_THAT(queue.as_string(), Ne("10 value: 10\n"));
}

TEST(PRQueueUsing, dequeueMultipleNodesTest) {
  prqueue<int> queue;
  queue.enqueue(1, 5);
  queue.enqueue(2, 3);
  queue.enqueue(3, 8);
  queue.enqueue(4, 4);
  queue.dequeue();

  EXPECT_THAT(queue.peek(), Eq(4));
}

TEST(PRQueueDuplicates, enqueueDuplicatesTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 100);
  queue.enqueue(3, 300);

  auto root = queue.getRoot();

  ASSERT_NE(root, (nullptr));
  EXPECT_THAT(root->priority, Eq(100));
  EXPECT_THAT(root->value, Eq(1));

  ASSERT_NE(root->link, nullptr);
  EXPECT_THAT(root->link->priority, Eq(100));
  EXPECT_THAT(root->link->value, Eq(2));

  EXPECT_THAT(queue.size(), Eq(3));
}

TEST(PRQueueDuplicates, DequeueDuplicatesTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(1, 100);
  queue.enqueue(3, 300);

  EXPECT_THAT(queue.dequeue(), Eq(1));
  EXPECT_THAT(queue.dequeue(), Eq(1));
  EXPECT_THAT(queue.size(), Eq(1));
}

TEST(PRQueueDuplicates, asStringDuplicatesTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 100);
  queue.enqueue(3, 300);

  EXPECT_THAT(queue.as_string(),
              Eq("100 value: 1\n100 value: 2\n300 value: 3\n"));
}

TEST(PRQueueDuplicates, clearWithDuplicatesTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 100);
  queue.enqueue(3, 300);

  EXPECT_THAT(queue.size(), Eq(3));

  queue.clear();

  EXPECT_THAT(queue.size(), Eq(0));
}

TEST(PRQueueCopying, assignmentOperatorTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 100);
  queue.enqueue(3, 300);

  prqueue<int> queue2;

  queue2 = queue;

  EXPECT_THAT(queue2.size(), Eq(queue.size()));
  EXPECT_NE(queue2.getRoot(), nullptr);

  ASSERT_THAT(queue2.getRoot()->priority, Eq(100));
  ASSERT_THAT(queue2.getRoot()->value, Eq(1));

  EXPECT_THAT(queue2.getRoot()->link->priority, Eq(100));
  EXPECT_THAT(queue2.getRoot()->link->value, Eq(2));
  EXPECT_THAT(queue2.peek(), Eq(queue.peek()));
  EXPECT_NE(queue2.getRoot(), (queue.getRoot()));
}

TEST(PRQueueCopying, copyConstructorTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 100);
  queue.enqueue(2, 300);

  prqueue<int> queue2(queue);

  EXPECT_THAT(queue2.size(), Eq(queue.size()));

  EXPECT_NE(queue2.getRoot(), nullptr);
  ASSERT_THAT(queue2.getRoot()->priority, Eq(100));
  EXPECT_THAT(queue2.getRoot()->value, Eq(1));
  EXPECT_NE(queue2.getRoot()->link, nullptr);

  EXPECT_THAT(queue2.peek(), Eq(queue.peek()));
  EXPECT_NE(queue2.getRoot(), (queue.getRoot()));
}

TEST(PRQueueEquality, operatorEqualTest) {
  prqueue<int> queue;
  queue.enqueue(1, 100);
  queue.enqueue(2, 200);
  queue.enqueue(3, 300);

  prqueue<int> queue2;
  queue2.enqueue(1, 100);
  queue2.enqueue(2, 200);
  queue2.enqueue(3, 300);

  EXPECT_THAT(queue == queue2, Eq(true));

  prqueue<int> queue3;
  queue3.enqueue(1, 200);
  queue3.enqueue(2, 200);
  queue3.enqueue(4, 300);
  EXPECT_THAT(queue2 == queue3, Eq(false));

  queue2.enqueue(4, 400);
  EXPECT_THAT(queue == queue2, Eq(false));
}

TEST(PRQueueIterator, beginTest) {
  prqueue<int> queue;
  int value = 0, priority = 0;
  queue.begin();
  EXPECT_THAT(queue.next(value, priority), Eq(false));

  queue.enqueue(10, 100);
  queue.begin();
  EXPECT_THAT(queue.next(value, priority), Eq(true));
  EXPECT_THAT(priority, Eq(100));
  EXPECT_THAT(value, Eq(10));
  EXPECT_THAT(queue.next(value, priority), Eq(false));
}

TEST(PRQueueIterator, nextTest) {
  prqueue<int> queue;

  int val = 0, priority = 0;
  queue.enqueue(1, 4);
  queue.enqueue(2, 2);
  queue.enqueue(3, 3);
  queue.enqueue(4, 1);
  queue.enqueue(5, 6);
  queue.enqueue(6, 5);
  queue.enqueue(7, 7);
  queue.enqueue(8, 8);
  queue.begin();
  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(1));
  EXPECT_THAT(val, Eq(4));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(2));
  EXPECT_THAT(val, Eq(2));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(3));
  EXPECT_THAT(val, Eq(3));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(4));
  EXPECT_THAT(val, Eq(1));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(5));
  EXPECT_THAT(val, Eq(6));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(6));
  EXPECT_THAT(val, Eq(5));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(7));
  EXPECT_THAT(val, Eq(7));

  EXPECT_THAT(queue.next(val, priority), Eq(true));
  EXPECT_THAT(priority, Eq(8));
  EXPECT_THAT(val, Eq(8));

  EXPECT_THAT(queue.next(val, priority), Eq(false));
  EXPECT_THAT(queue.next(val, priority), Eq(false));
}