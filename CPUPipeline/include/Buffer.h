#pragma once
#include <mutex>
#include <queue>
template<class T>
class Buffer
{
public:
	void add(T * obj) {
		while (true) {
			std::unique_lock<std::mutex> locker(mu);
			cond.wait(locker, [this]() {return buffer_.size() < size_; });
			buffer_.push(obj);
			locker.unlock();
			cond.notify_all();
			return;
		}
	}
	T * remove() {
		while (true)
		{
			std::unique_lock<std::mutex> locker(mu);
			cond.wait(locker, [this]() {return buffer_.size() > 0; });
			T* back = buffer_.front();
			buffer_.pop();
			locker.unlock();
			cond.notify_all();
			emptyCond.notify_all();
			return back;
		}
	}

	void waitUntilEmpty() {
		std::unique_lock <std::mutex> locker(emptyMu);
		cond.wait(locker, [this]() {return buffer_.size() == 0;});
		locker.unlock();
		emptyCond.notify_all();
	}
	Buffer() {}
private:
	std::mutex mu;
	std::condition_variable cond;

	std::mutex emptyMu;
	std::condition_variable emptyCond;

	std::queue<T *> buffer_;
	const unsigned int size_ = 10;
};