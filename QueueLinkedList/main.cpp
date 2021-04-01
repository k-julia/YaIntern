#include <iostream>
#include <queue>
#include "queue_linked_list.h"

using namespace containers;

int main() {
  QueueLinkedList<int> q1({1,2,3,4,1,6});
  std::cout << q1.Find(7) << std::endl;
  while (!q1.IsEmpty()) {
    std::cout << q1.Pop() << " ";
  }
  std::queue<int> v;
  for (int i = 0; i < 1e6; ++i) {
    int opcode{rand() % 2};
    if (v.size() == 0 || opcode == 1) {
      int num{rand()};
      q1.Push(num);
      v.push(num);
    } else {
      if (q1.Pop() != v.front()) {
        std::cout << "Zhopa";
      }
      v.pop();
    }
  }
  return 0;
}
