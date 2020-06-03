#include "PrimitiveThreadsPool.h"


PrimitiveThreadsPool::PrimitiveThreadsPool(unsigned int tnumber): end(false) {
	for (unsigned int i = 0; i < tnumber; ++i) {

		threads.emplace_back(std::thread(
			[this]() {
				for (;;) {

					std::function<void()> task; //our future task

					{
						std::unique_lock<std::mutex> ul(this->mtx); //lock mutex
						cv.wait(ul, [this]() {return !this->tasks.empty() || this->end; }); //wait until notify and check if vector is not empty

						if (this->end)
							return; //stop thread if end 

						task = std::move(this->tasks.front()); //if task was added to vector tak it
						this->tasks.pop(); //remove the task we took from vector
					}

					task(); //run task
				}
			}));
	}
}



void PrimitiveThreadsPool::AddTask(void(*f)()) {

	tasks.emplace(*f); //add task to queue

	cv.notify_one(); //notify waiting threads about tasks
}



PrimitiveThreadsPool::~PrimitiveThreadsPool() {

	end = true;
	
	cv.notify_all();

	for (auto& x : threads) 
	{ 
		x.join();
	}
}