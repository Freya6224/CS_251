#include "canvaslist.h"

#include <iostream>

using namespace std;

CanvasList::CanvasList() : list_size(0), list_front(nullptr) {
}

CanvasList::CanvasList(const CanvasList& other)
    : list_size(0), list_front(nullptr) {
  if (other.list_front == nullptr) {
    return;
  }
  list_front = new ShapeNode();
  list_front->value = other.list_front->value->copy();
  list_size++;

  ShapeNode* curOther = other.list_front->next;
  ShapeNode* curCopy = list_front;

  while (curOther != nullptr) {
    ShapeNode* createNewNode = new ShapeNode();
    createNewNode->value = curOther->value->copy();
    createNewNode->next = nullptr;

    curCopy->next = createNewNode;
    curCopy = createNewNode;

    curOther = curOther->next;
    list_size++;
  }
}
CanvasList::~CanvasList() {
  clear();
}
CanvasList& CanvasList::operator=(const CanvasList& other) {
  if (this == &other) {
    return *this;
  }

  clear();

  if (other.list_size == 0) {
    return *this;
  }

  ShapeNode* cur = other.list_front;
  ShapeNode* node = nullptr;
  while (cur != nullptr) {
    ShapeNode* newNode = new ShapeNode();
    newNode->value = cur->value->copy();
    newNode->next = nullptr;
    if (list_front == nullptr) {
      list_front = newNode;
    } else {
      node->next = newNode;
    }
    node = newNode;
    cur = cur->next;
    list_size++;
  }
  return *this;
}
ShapeNode* CanvasList::front() const {
  return list_front;
}
bool CanvasList::empty() const {
  return list_size == 0;
}
int CanvasList::size() const {
  return list_size;
}
void CanvasList::push_front(Shape* shape) {
  ShapeNode* createNode = new ShapeNode();
  createNode->value = shape;
  if (empty()) {
    createNode->next = nullptr;
    list_front = createNode;
    list_size = 1;
  } else {
    createNode->next = list_front;
    list_front = createNode;
    list_size++;
  }
}
void CanvasList::clear() {
  if (empty()) {
    return;
  }
  ShapeNode* curNode = list_front;
  while (curNode != nullptr) {
    ShapeNode* tempNode = curNode;
    curNode = curNode->next;
    delete tempNode->value;
    delete tempNode;
  }
  list_front = nullptr;
  list_size = 0;
}
void CanvasList::insert_after(int num, Shape* shape) {
  if (list_size <= num || num < 0) {
    return;
  }

  int insertAt = 0;
  ShapeNode* cur = list_front;

  while (insertAt != num) {
    insertAt++;
    cur = cur->next;
  }

  ShapeNode* newNode = new ShapeNode();
  newNode->value = shape;
  newNode->next = cur->next;
  cur->next = newNode;
  list_size++;
}

void CanvasList::push_back(Shape* shape) {
  ShapeNode* createNewNode = new ShapeNode();
  createNewNode->value = shape;
  createNewNode->next = nullptr;

  if (list_front == nullptr) {
    list_front = createNewNode;
  } else {
    ShapeNode* cur = list_front;
    while (cur->next != nullptr) {
      cur = cur->next;
    }
    cur->next = createNewNode;
  }
  list_size++;
}

void CanvasList::remove_at(int num) {
  if (empty() || num >= list_size || num < 0) {
    return;
  }

  if (list_size == 1) {
    clear();
    return;
  }

  list_size -= 1;
  if (num == 0) {
    ShapeNode* cur = list_front;
    list_front = list_front->next;
    delete cur->value;
    delete cur;
    return;
  }

  int removeIndAt = 1;
  ShapeNode* node = list_front;
  ShapeNode* cur = list_front->next;

  while (removeIndAt != num) {
    removeIndAt++;
    node = cur;
    cur = cur->next;
  }

  node->next = cur->next;

  delete cur->value;
  delete cur;
}
void CanvasList::remove_every_other() {
  if (empty() || list_size == 1) {
    return;
  }

  ShapeNode* prev = list_front;
  ShapeNode* cur = list_front->next;

  int removeAt = 1;

  while (cur != nullptr) {
    remove_at(removeAt);
    prev = prev->next;
    if (prev == nullptr) {
      return;
    }
    cur = prev->next;
    removeAt++;
  }
}

Shape* CanvasList::pop_front() {
  if (empty()) {
    return nullptr;
  }

  Shape* shape = list_front->value;
  ShapeNode* cur = list_front->next;

  delete list_front;

  list_front = cur;
  list_size--;

  return shape;
}

Shape* CanvasList::pop_back() {
  if (empty()) {
    return nullptr;
  }

  if (list_size == 1) {
    return pop_front();
  }

  ShapeNode* node = list_front;
  ShapeNode* cur = list_front->next;

  while (cur->next != nullptr) {
    node = cur;
    cur = cur->next;
  }

  node->next = nullptr;
  Shape* shape = cur->value;

  delete cur;
  list_size--;

  return shape;
}

int CanvasList::find(int x, int y) const {
  ShapeNode* cur = list_front;
  int num = 0;
  while (cur != nullptr) {
    if (cur->value->getX() == x && cur->value->getY() == y) {
      return num;
    }
    cur = cur->next;
    num++;
  }
  return -1;
}

Shape* CanvasList::shape_at(int num) const {
  if (num >= list_size || num < 0) {
    return nullptr;
  }

  ShapeNode* cur = list_front;
  for (int i = 0; i < num; ++i) {
    cur = cur->next;
  }
  return cur->value;
}

void CanvasList::draw() const {
  ShapeNode* cur = list_front;
  while (cur != nullptr) {
    cout << cur->value->as_string() << endl;
    cur = cur->next;
  }
}

void CanvasList::print_addresses() const {
  ShapeNode* cur = list_front;
  while (cur != nullptr) {
    cout << "Node Address: " << cur << "\tShape Address: " << cur->value
         << endl;
    cur = cur->next;
  }
}
