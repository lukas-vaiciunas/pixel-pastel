#pragma once

#include <functional>
#include <mutex>
#include <queue>

class ThreadPool
{
public:
	ThreadPool(const ThreadPool &) = delete;
	ThreadPool(ThreadPool &&) = delete;

	ThreadPool &operator=(const ThreadPool &) = delete;
	ThreadPool &operator=(ThreadPool &&) = delete;

	static ThreadPool &getInstance();

	void start();
	void push(const std::function<void()> &job);
	void stop();
	bool busy();
private:
	std::vector<std::thread> threads_;
	std::queue<std::function<void()>> jobs_;

	std::mutex jobsMutex_;
	std::condition_variable mutexCondition_;
	
	bool shouldTerminate_;

	ThreadPool();
	~ThreadPool() {}

	void loop_();
};