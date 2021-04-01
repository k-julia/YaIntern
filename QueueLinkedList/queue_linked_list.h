#pragma once

#include <iostream>
#include <vector>
#include <cassert>

namespace containers {

template<typename T>
class QueueLinkedList {
 public:
  struct Node {
   public:
    template<typename>
    friend
    class QueueLinkedList;

    T value;

   private:
    Node(T value, Node* next) : value(std::move(value)), next_(next) {}

    Node* next_;
  };

  QueueLinkedList() = default;
  QueueLinkedList(const std::initializer_list<T>& list) {
    for (const auto& element : list) {
      Push(element);
    }
  }
  QueueLinkedList(const QueueLinkedList& rhs) {
    CopyFrom(rhs);
  }
  QueueLinkedList(QueueLinkedList&& rhs) noexcept {
    MoveFrom(std::move(rhs));
  }

  ~QueueLinkedList() {
    SafeDelete();
  }

  int Size() const { return size_; }
  bool IsEmpty() const { return size_ == 0; }

  std::vector<T> ToVector() const {
    std::vector<T> output;
    for (Node* ptr = first_; ptr; ptr = ptr->next_) {
      output.push_back(ptr->value);
    }
    return output;
  }

  Node* Head() { return first_; }
  const Node* Head() const { return first_; }
  Node* Tail() { return last_; }
  const Node* Tail() const { return last_; }

  void Push(const T& value) { PushTo(new Node(value, nullptr)); }
  void Push(T&& value) { PushTo(new Node(std::move(value), nullptr)); }

  T Pop() {
    if (size_ == 0) {
      throw std::exception();
    }
    Node* source_ptr = first_;
    T source_value = std::move(source_ptr->value);
    first_ = first_->next_;
    delete source_ptr;
    size_--;
    if (size_ == 0) {
      last_ = nullptr;
      first_ = nullptr;
    }
    return source_value;
  }

  int Find(const T& value) const {
    int position = 0;
    for (Node* ptr = first_; ptr; ptr = ptr->next_) {
      if (ptr->value == value) {
        return position;
      }
      position++;
    }
    return -1;
  }
  std::vector<int> FindAll(const T& value) const {
    std::vector<int> positions;
    int position = 0;
    for (Node* ptr = first_; ptr; ptr = ptr->next_) {
      if (ptr->value == value) {
        positions.push_back(position);
      }
      position++;
    }
    return positions;
  }

  Node* operator[](int position) {
    return At(position);
  }
  const Node* operator[](int position) const {
    return At(position);
  }

  bool operator==(const QueueLinkedList<T>& rhs) const {
    if (size_ != rhs.size_) {
      return false;
    }
    Node* ptr1 = first_;
    Node* ptr2 = rhs.first_;
    for (int i = 0; i < size_; i++) {
      if (ptr1->value != ptr2->value) {
        return false;
      }
      ptr1 = ptr1->next_;
      ptr2 = ptr2->next_;
    }
    return true;
  }
  bool operator!=(const QueueLinkedList<T>& rhs) const {
    return !(*this == rhs);
  }

  QueueLinkedList<T>& operator=(const QueueLinkedList<T>& rhs) {
    if (this == &rhs) {
      return *this;
    }
    SafeDelete();
    CopyFrom(rhs);
    return *this;
  }
  QueueLinkedList<T>& operator=(QueueLinkedList<T>&& rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }
    SafeDelete();
    MoveFrom(std::move(rhs));
    return *this;
  }

 private:
  void SafeDelete() {
    while (size_ != 0) {
      Pop();
    }
  }

  void CopyFrom(const QueueLinkedList<T>& rhs) {
    for (Node* ptr = rhs.first_; ptr; ptr = ptr->next_) {
      Push(ptr->value);
    }
  }
  void MoveFrom(QueueLinkedList<T>&& rhs) {
    first_ = rhs.first_;
    last_ = rhs.last_;
    size_ = rhs.size_;
    rhs.first_ = nullptr;
    rhs.last_ = nullptr;
    rhs.size_ = 0;
  }
  Node* At(int position) const {
    assert(position < size_);
    assert(position >= 0);
    Node* ptr = first_;
    for (int i = 0; i != position; i++) {
      ptr = ptr->next_;
    }
    return ptr;
  }
  void PushTo(Node* ptr) {
    if (size_ == 0) {
      first_ = ptr;
    } else {
      last_->next_ = ptr;
    }
    last_ = ptr;
    size_++;
  }

  Node* first_ = nullptr;
  Node* last_ = nullptr;
  size_t size_ = 0;
};

}  // namespace containers
