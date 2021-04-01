#include <fstream>
#include "long_number.h"

int main() {
  std::ifstream stream("input");
  big_integer a;
  stream >> a;
  big_integer b;
  stream >> b;
  big_integer c;
  c = a % b;
  std::cout << c;
  return 0;
}
