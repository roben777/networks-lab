#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

// cv is global
std::condition_variable cv;

void sleeper() {
  // create a unique lock for the wait function and a mutex needed by unique lock
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "Sleeper waits\n" << std::flush;
  cv.wait(lock);
  std::cout << "Sleeper resumes\n" << std::flush;
}


void waker() {
  // we first sleep for 1 sec
  std::cout << "Waker waits for 1 sec\n" << std::flush;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "Waker notifies\n" << std::flush;
  cv.notify_one();
}


int main() {
  std::thread tsleeper(sleeper);
  std::thread twaker(waker);

  tsleeper.join();
  twaker.join();
 
  return 0;
}
