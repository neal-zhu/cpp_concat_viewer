#include <vector>
#include <iostream>

#include "concat_viewer.h"

int main() {
  std::vector<int> v1{1, 2, 3, 4};
  std::vector<int> v2{1, 2, 3};
  auto v3 = v1;
  auto v4 = v2;
  ConcatViewer<std::vector<int>::iterator> c;
  c.Concat(v1.begin(), v1.end());
  c.Concat(v4.begin(), v4.begin());
  c.Concat(v2.begin(), v2.end());
  c.Concat(v1.begin(), v1.begin() + 2);
  c.Concat(v3.end(), v3.end());
  for (auto it = c.begin(); it != c.end(); it++) {
    std::cout << *it << " ";
    (*it)++;
  }
  std::cout << "\n";

  for (auto it = c.begin(); it != c.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << "\n";
}
