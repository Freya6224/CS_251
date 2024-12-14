#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
 public:
  struct NODE {
    int priority;
    T value;
    NODE* parent;
    NODE* left;
    NODE* right;
    NODE* link;  // Link to duplicates -- Part 2 only
  };

 private:
  NODE* root;
  size_t sz;

  // Utility pointers for begin and next.
  NODE* curr;
  NODE* temp;  // Optional

  // TODO_STUDENT: add private helper function definitions here
  void clearHelperFunct(NODE* node) {
    if (node) {
      clearHelperFunct(node->left);
      clearHelperFunct(node->right);

      NODE* duplicate = node->link;
      while (duplicate) {
        NODE* duplicate2 = duplicate->link;
        delete duplicate;
        duplicate = duplicate2;
      }
      delete node;
    }
  }
  NODE* findMin(NODE* node) const {
    while (node != nullptr && node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  void stringHelper(NODE* node, ostringstream& osStream) const {
    if (node == nullptr) {
      return;
    }
    stringHelper(node->left, osStream);
    osStream << node->priority << " value: " << node->value << "\n";
    NODE* duplicate = node->link;
    while (duplicate) {
      osStream << duplicate->priority << " value: " << duplicate->value << "\n";
      duplicate = duplicate->link;
    }
    stringHelper(node->right, osStream);
  }

  NODE* copyHelper(NODE* other, NODE* parent) {
    if (!other) {
      return nullptr;
    }
    NODE* node2 = new NODE;
    node2->priority = other->priority;
    node2->value = other->value;
    node2->parent = parent;

    node2->link = copyHelper(other->link, node2);
    node2->left = copyHelper(other->left, node2);
    node2->right = copyHelper(other->right, node2);
    return node2;
  }

  bool equalHelper(NODE* node, NODE* node1) const {
    if (node == nullptr && node1 == nullptr) {
      return true;
    } else if (node == nullptr || node1 == nullptr) {
      return false;
    } else if (node->priority != node1->priority ||
               node->value != node1->value) {
      return false;
    } else {
      return equalHelper(node->left, node1->left) &&
             equalHelper(node->right, node1->right) &&
             equalHelper(node->link, node1->link);
    }
  }

 public:
  /// Creates an empty `prqueue`.
  ///
  /// Runs in O(1).
  prqueue() : root(nullptr), sz(0) {
  }

  /// Returns the number of elements in the `prqueue`.
  ///
  /// Runs in O(1).
  size_t size() const {
    return sz;
  }

  /// Returns a pointer to the root node of the BST.
  ///
  /// Used for testing the internal structure of the BST. Do not edit or
  /// change.
  ///
  /// Runs in O(1).
  NODE* getRoot() {
    return root;
  }

  /// Adds `value` to the `prqueue` with the given `priority`.
  ///
  /// Uses the priority to determine the location in the underlying tree.
  ///
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  void enqueue(T value, int priority) {
    NODE* node = new NODE;
    node->priority = priority;
    node->value = value;
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->link = nullptr;
    if (root == nullptr) {
      root = node;

    } else {
      NODE* curr = root;
      NODE* temp = nullptr;

      while (curr != nullptr) {
        temp = curr;
        if (priority < curr->priority) {
          curr = curr->left;
        } else if (priority > curr->priority) {
          curr = curr->right;
        } else {
          while (curr->link != nullptr) {
            curr = curr->link;
          }
          node->parent = curr;
          curr->link = node;
          ++sz;
          return;
        }
      }

      if (priority < temp->priority) {
        temp->left = node;
      } else {
        temp->right = node;
      }
      node->parent = temp;
    }
    ++sz;
  }

  /// Returns the value with the smallest priority in the `prqueue`, but does
  /// not modify the `prqueue`.
  ///
  /// If the `prqueue` is empty, returns the default value for `T`.
  ///
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  T peek() const {
    if (!root) {
      return T{};
    }

    NODE* frontNode = findMin(root);
    return frontNode->value;
  }

  /// Returns the value with the smallest priority in the `prqueue` and
  /// removes it from the `prqueue`.
  ///
  /// If the `prqueue` is empty, returns the default value for `T`.
  ///
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  T dequeue() {
    if (!root) {
      return T{};
    }

    NODE* curr = findMin(root);
    T getValue = curr->value;

    if (curr->link) {
      curr->link->parent = curr->parent;
      curr->link->left = curr->left;
      curr->link->right = curr->right;

      if (curr == root) {
        root = curr->link;
      } else {
        if (curr == curr->parent->left) {
          curr->parent->left = curr->link;
        } else {
          curr->parent->right = curr->link;
        }
      }
      if (curr->right) {
        curr->right->parent = curr->link;
      }

      delete curr;
    } else {
      if (curr == root) {
        root = curr->right;
      } else if (curr == curr->parent->left) {
        curr->parent->left = curr->right;
      } else {
        curr->parent->right = curr->right;
      }

      if (curr->right) {
        curr->right->parent = curr->parent;
      }

      delete curr;
    }

    sz--;
    return getValue;
  }

  /// Converts the `prqueue` to a string representation, with the values
  /// in-order by priority.
  ///
  /// Example:
  ///
  /// ```c++
  /// prqueue<string> names;
  /// names.enqueue("Gwen", 3);
  /// names.enqueue("Jen", 2);
  /// names.enqueue("Ben", 1);
  /// names.enqueue("Sven", 2);
  /// ```
  ///
  /// Calling `names.as_string()` would return the following multi-line
  /// string:
  ///
  /// ```text
  /// 1 value: Ben
  /// 2 value: Jen
  /// 2 value: Sven
  /// 3 value: Gwen
  /// ```
  ///
  /// Runs in O(N), where N is the number of values.
  string as_string() const {
    ostringstream osStream;
    stringHelper(root, osStream);
    return osStream.str();
  }

  /// Empties the `prqueue`, freeing all memory it controls.
  ///
  /// Runs in O(N), where N is the number of values.
  void clear() {
    clearHelperFunct(root);
    root = nullptr;
    sz = 0;
  }

  /// Destructor, cleans up all memory associated with `prqueue`.
  ///
  /// Runs in O(N), where N is the number of values.
  ~prqueue() {
    clear();
  }

  /// Copy constructor.
  ///
  /// Copies the value-priority pairs from the provided `prqueue`.
  /// The internal tree structure must be copied exactly.
  ///
  /// Runs in O(N), where N is the number of values in `other`.
  prqueue(const prqueue& other) : root(nullptr), sz(0) {
    // TODO_STUDENT
    if (other.root != nullptr) {
      root = copyHelper(other.root, nullptr);
      sz = other.sz;
    }
  }

  /// Assignment operator; `operator=`.
  ///
  /// Clears `this` tree, and copies the value-priority pairs from the
  /// provided `prqueue`. The internal tree structure must be copied exactly.
  ///
  /// Runs in O(N + O), where N is the number of values in `this`, and O is
  /// the number of values in `other`.
  prqueue& operator=(const prqueue& other) {
    // TODO_STUDENT
    if (this != &other) {
      clear();
      root = copyHelper(other.root, nullptr);
      sz = other.sz;
    }
    return *this;
  }

  /// Checks if the contents of `this` and `other` are equivalent.
  ///
  /// Two `prqueues` are equivalent if they have the same priorities and
  /// values, as well as the same internal tree structure.
  ///
  /// These two `prqueue`s would be considered equivalent, because
  /// they have the same internal tree structure:
  ///
  /// ```c++
  /// prqueue<string> a;
  /// a.enqueue("2", 2);
  /// a.enqueue("1", 1);
  /// a.enqueue("3", 3);
  /// ```
  ///
  /// and
  ///
  /// ```c++
  /// prqueue<string> b;
  /// a.enqueue("2", 2);
  /// a.enqueue("3", 3);
  /// a.enqueue("1", 1);
  /// ```
  ///
  /// While this `prqueue`, despite containing the same priority-value pairs,
  /// would not be considered equivalent, because the internal tree structure
  /// is different.
  ///
  /// ```c++
  /// prqueue<string> c;
  /// a.enqueue("1", 1);
  /// a.enqueue("2", 2);
  /// a.enqueue("3", 3);
  /// ```
  ///
  /// Runs in O(N) time, where N is the maximum number of nodes in
  /// either `prqueue`.
  ///
  bool operator==(const prqueue& other) const {
    // TODO_STUDENT
    if (sz != other.sz) {
      return false;
    }
    return equalHelper(root, other.root);
  }

  /// Resets internal state for an iterative inorder traversal.
  ///
  /// See `next` for usage details.
  ///
  /// O(H), where H is the maximum height of the tree.
  void begin() {
    curr = root;
    if(curr == nullptr)
    {
      return;
    }
    while (curr->left != nullptr) {
      curr = curr->left;
    }
  }

  /// Uses the internal state to return the next in-order value and priority
  /// by reference, and advances the internal state. Returns true if the
  /// reference parameters were set, and false otherwise.
  ///
  /// Example usage:
  ///
  /// ```c++
  /// pq.begin();
  /// T value;
  /// int priority;
  /// while (pq.next(value, priority)) {
  ///   cout << priority << " value: " << value << endl;
  /// }
  /// ```
  ///
  /// Runs in worst-case O(H + M) or O(H), depending on implementation, where
  /// H is the height of the tree, and M is the number of duplicate
  /// priorities.
  bool next(T& value, int& priority) {
    if (curr == nullptr) {
      return false;
    }

    value = curr->value;
    priority = curr->priority;

    if (curr->link != nullptr) {
      curr = curr->link;
      return true;
    }
    if (curr->right != nullptr) {
      curr = curr->right;
      while (curr->left != nullptr) {
        curr = curr->left;
      }
    } else {
      while (curr->parent != nullptr &&
             curr->priority == curr->parent->priority) {
        curr = curr->parent;
      }
      if (curr->right != nullptr) {
        curr = curr->right;
        while (curr->left != nullptr) {
          curr = curr->left;
        }
      } else {
        while (curr->parent != nullptr) {
          if (curr != curr->parent->right) {
            curr = curr->parent;
            return true;
          }
          curr = curr->parent;
        }
        curr = curr->parent;
      }
    }
    return true;
  }
};
