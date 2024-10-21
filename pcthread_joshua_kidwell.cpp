#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#define MAX 10
std::mutex the_mutex;
std::condition_variable condc, condp;
int buffer = 0;

void producer() {
	for (int i = 1; i <= MAX; i++) {
		std::unique_lock<std::mutex> lock(the_mutex);
		condp.wait(lock, [] {return buffer == 0;});
		buffer = i;
		std::cout << "Producer buffer: " << buffer << std::endl;
		condc.notify_one();
	}
}

void consumer()
{
	for (int i = 1; i <= MAX; i++) {
		std::unique_lock<std::mutex> lock(the_mutex);
		condc.wait(lock, [] {return buffer != 0;});
		std::cout << "Consumer buffer: " << buffer << std::endl;
		buffer = 0;
		condp.notify_one();
	}
}

int main() 
{
	std::thread pro(producer);
	std::thread con(consumer);

	pro.join();
	con.join();

	return 0;
}
