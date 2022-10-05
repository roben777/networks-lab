#include <iostream>
#include <condition_variable>
#include <thread>

// cv is now a global variable
std::condition_variable cv;

// notifier is executed by parallel threads
// id is the ID of the thread
void notifier(int id) {
  for(int i=0; i<10; i++) {
    std::cout << "Thread " << id << ", notification nr " << i << std::endl;
    cv.notify_one();
  }
}

int main() {
  std::thread one(notifier, 0);
  std::thread two(notifier, 1);

  one.join();
  two.join();
  
  return 0;
}
