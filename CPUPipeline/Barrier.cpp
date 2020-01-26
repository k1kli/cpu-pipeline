#include "Barrier.h"

//https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
Barrier::Barrier(int iCount) :
	mThreshold(iCount),
	mCount(iCount),
	mGeneration(0) {
}

void Barrier::Wait() {
	std::unique_lock<std::mutex> lLock{ mMutex };
	auto lGen = mGeneration;
	if (!--mCount) {
		mGeneration++;
		mCount = mThreshold;
		mCond.notify_all();
	}
	else {
		mCond.wait(lLock, [this, lGen] { return lGen != mGeneration; });
	}
}
