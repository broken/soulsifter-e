#ifndef __soul_sifter__JobQueue__
#define __soul_sifter__JobQueue__

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <tuple>

namespace dogatech {

template <typename ReturnT>
class JobQueue {
 public:
  JobQueue(size_t max_threads = 1);

  std::future<ReturnT> push(std::function<ReturnT()> job);

 private:
  const size_t max_threads;
  size_t num_threads;
  std::queue<std::tuple<std::function<ReturnT()>, std::promise<ReturnT>>> jobs;
  std::mutex mutex;

  void pop();

  class JobLock {
   public:
    explicit JobLock(JobQueue&);
    ~JobLock();
    void lock();
    void unlock();
   private:
    JobQueue& q;
    bool hasLock;
  };
};


template <typename ReturnT>
JobQueue<ReturnT>::JobQueue(size_t max_threads) : max_threads(max_threads), num_threads(0) {
}

template <typename ReturnT>
std::future<ReturnT> JobQueue<ReturnT>::push(std::function<ReturnT()> job) {
  std::promise<ReturnT> promise;
  std::future<ReturnT> future = promise.get_future();
  std::lock_guard<std::mutex> lock(mutex);
  jobs.emplace(std::move(job), std::move(promise));
  if (num_threads < max_threads) {
    std::thread t(&JobQueue<ReturnT>::pop, this);
    t.detach();
    ++num_threads;
  }
  return future;
}

template <typename ReturnT>
void JobQueue<ReturnT>::pop() {
  JobLock lock(*this);
  while (jobs.size() > 0) {
    auto job = std::move(jobs.front());
    jobs.pop();
    lock.unlock();

    auto& func = std::get<0>(job);
    auto& promise = std::get<1>(job);
    promise.set_value(func());

    lock.lock();
  }
}

template <typename ReturnT>
JobQueue<ReturnT>::JobLock::JobLock(JobQueue& jobQueue) : q(jobQueue), hasLock(true) {
  q.mutex.lock();
}

template <typename ReturnT>
JobQueue<ReturnT>::JobLock::~JobLock() {
  if (!hasLock) q.mutex.lock();
  --q.num_threads;
  q.mutex.unlock();
}

template <typename ReturnT>
void JobQueue<ReturnT>::JobLock::lock() {
  q.mutex.lock();
  hasLock = true;
}

template <typename ReturnT>
void JobQueue<ReturnT>::JobLock::unlock() {
  q.mutex.unlock();
  hasLock = false;
}

}  // namespace dogatech

#endif /* defined(__soul_sifter__JobQueue__) */
