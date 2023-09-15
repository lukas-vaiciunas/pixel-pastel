#include "ThreadPool.h"

ThreadPool::ThreadPool() :
	threads_(),
	jobs_(),
	jobsMutex_(),
	mutexCondition_(),
	shouldTerminate_(false)
{}

ThreadPool &ThreadPool::getInstance()
{
	static ThreadPool instance;
	return instance;
}

void ThreadPool::start()
{
	const uint32_t numThreads = std::thread::hardware_concurrency();

	for (uint32_t i = 0; i < numThreads; ++i)
	{
		threads_.emplace_back(std::thread(&ThreadPool::loop_, this));
	}
}

void ThreadPool::push(const std::function<void()> &job)
{
	{
		std::unique_lock<std::mutex> lock(jobsMutex_);
		jobs_.push(job);
	}

	mutexCondition_.notify_one();
}

void ThreadPool::stop()
{
	{
		std::unique_lock<std::mutex> lock(jobsMutex_);
		shouldTerminate_ = true;
	}

	mutexCondition_.notify_all();

	for (std::thread &thread : threads_)
	{
		thread.join();
	}

	threads_.clear();
}

bool ThreadPool::busy()
{
	bool busy;

	{
		std::unique_lock<std::mutex> lock(jobsMutex_);
		busy = !jobs_.empty();
	}

	return busy;
}

void ThreadPool::loop_()
{
	while (true)
	{
		std::function<void()> job;

		{
			std::unique_lock<std::mutex> lock(jobsMutex_);
			mutexCondition_.wait(lock, [this] { return !jobs_.empty() || shouldTerminate_; });
			if (shouldTerminate_)
			{
				return;
			}
			job = jobs_.front();
			jobs_.pop();
		}

		job();
	}
}