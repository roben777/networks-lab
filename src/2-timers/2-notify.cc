#include <iostream>
#include <condition_variable>

int main() {
  std::condition_variable cv;
  std::cout << "Before notify\n";
  cv.notify_one();
  std::cout << "After notify\n";
  
  return 0;
}
