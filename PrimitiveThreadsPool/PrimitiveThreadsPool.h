#pragma once
#include <thread>
#include<queue>
#include<mutex>
#include<functional>
#include<condition_variable>

class PrimitiveThreadsPool
{
public:

	PrimitiveThreadsPool(unsigned int); //create number of threads
	PrimitiveThreadsPool(const PrimitiveThreadsPool&) = delete;
	PrimitiveThreadsPool(PrimitiveThreadsPool&&) = delete;

	~PrimitiveThreadsPool();

	PrimitiveThreadsPool& operator=(const PrimitiveThreadsPool&) = delete;
	PrimitiveThreadsPool& operator=(PrimitiveThreadsPool&&) = delete;

	void AddTask(void(*f)()); //add new task


private:

	std::queue<std::function<void()>> tasks;

	std::vector<std::thread> threads;

	std::mutex mtx;
	std::condition_variable cv,c;

	bool end;
};

