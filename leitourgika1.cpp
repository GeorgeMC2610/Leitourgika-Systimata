#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define _GNU_SOURCE
#define LOG(x) std::cout << x << std::endl;

//the number of threads, and the random chance number, are all global variables
const int number_of_threads = 10;
pthread_t AllThreads[number_of_threads];
int x;

//this is the sample function every thread runs. It has the sleep() function, which helps us make the program run for several seconds.
void* ThreadStart(void* i)
{	
	if (x != 1)
	{
		LOG("The good thread let me run my process!");
	}
	else
	{
		LOG("The bad thread took over, and now it's running.");
	}
	sleep(1);
	LOG("Function ended!\n");
	
	pthread_exit(0);
}

int main()
{
	//random chance generation for the process to either be good or bad.
	srand(time(NULL));
	
	//creating the 10 threads.
	int status_allThreads;
	pthread_t AllThreads[number_of_threads];
	
	//we create the other two threads, the bad one and the good one
	pthread_t badThread;
	pthread_t goodThread;
	int goodThread_Status = pthread_create(&goodThread, NULL, ThreadStart, NULL);
	int badThread_Status  = pthread_create(&badThread,  NULL, ThreadStart, NULL);
	
	
	//then we create the loop in which the threads start to take place.
	for (int i = 0; i < number_of_threads; i++)
	{	
		x = rand() % 2;
		
		//when the random chance outputs 0, it means we run the good thread.
		if (x == 0)
		{
			//first it initializes the attributes of the thread to be run
			pthread_attr_t attributes;
			pthread_attr_init(&attributes);
			
			//then it creates it
			status_allThreads = pthread_create(&AllThreads[i], &attributes, ThreadStart, NULL);
			
			//and after that, we use the yield function to let the good thread run last, as it gives way to the other threads.
			goodThread_Status = pthread_yield();
			
			//and then, the main thread waits until the current thread is done doing its job
			pthread_join(AllThreads[i], NULL);
		}
		//but when the random chance outputs zero, it means that the bad thread takes place
		else
		{
			//in which the thread creates and initializes the attributes of the thread.
			pthread_attr_t attributes;
			pthread_attr_init(&attributes);
			
			status_allThreads = pthread_create(&AllThreads[i], &attributes, ThreadStart, NULL);
			
			//but then it destroys them, using the following function. Then makes the main function wait until the badthread is done doing its job.
			pthread_attr_destroy(&attributes);
			pthread_join(badThread, NULL);
		}
	}
	
	exit(0);
	return 0;
}