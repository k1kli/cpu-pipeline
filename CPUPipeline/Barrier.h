#pragma once
#include <mutex>

//https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
class Barrier {
public:
	Barrier(int iCount);

	void Wait();

private:
	std::mutex mMutex;
	std::condition_variable mCond;
	std::size_t mThreshold;
	std::size_t mCount;
	std::size_t mGeneration;
};

