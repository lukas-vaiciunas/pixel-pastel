#pragma once

#include <functional>
#include <mutex>
#include <queue>

class ThreadPool
{
public:
	ThreadPool();

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

	void loop_();
};