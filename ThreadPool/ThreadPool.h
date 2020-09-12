#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
//Example of thread pool

#include<thread> //for std::thread
#include<condition_variable> //for std::conditional_variable
#include<mutex> //for std::mutex
#include<future> //for std::future
#include<queue> //for std::queue
#include<functional> //for std::bind
#include<atomic> //for std::atomic
#include<type_traits> //for std::invoke_result c++17
#include"task_wrapper.hpp" //for task_wrapper

class ThreadPool
{
public:

	ThreadPool(); //on default creates 1 thread
	ThreadPool(const size_t); //creates number of threads
	ThreadPool(const ThreadPool&) = delete; //no copy constructor
	ThreadPool(ThreadPool&&) = delete; //no move constructor

	~ThreadPool();

	ThreadPool& operator=(const ThreadPool&) = delete; //no copy operator
	ThreadPool& operator=(ThreadPool&&) = delete; //no move operator

	void AddThread(const size_t n); //Increase number of threads in the pool

	void StopPool(); //Stop poooling

	size_t ThreadsNumber();//return the nuber of the working threads


	template<typename T, typename... Args>
	auto AddTask(T&& obj, Args&&... args)
		-> std::future<typename std::invoke_result<T,Args...>::type> { //return future which contains function return value

		using rtype = typename std::invoke_result<T,Args...>::type; // deduce function return type

		std::packaged_task<rtype()> ptask(std::bind(std::forward<T>(obj), std::forward<Args>(args)...));
		std::future<rtype> future = ptask.get_future(); //get future from packaged task

		{
			std::lock_guard<std::mutex> lg(mtx); //needed to safely add task to tasks container

			tasks.emplace(std::move(ptask)); //add task to task container.

		}

		cv.notify_one(); //notify waiting thread

		return future; //return future
	}


private:
	size_t nthreads; //number of threads

	std::atomic<bool> stop; //variable to check if we have to stop threading

	std::mutex mtx;

	std::condition_variable cv;

	std::queue<task_wrapper> tasks; //container for tasks

	std::vector<std::thread> threads; //container for threads

};
#endif