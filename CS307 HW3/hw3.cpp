#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <queue>
#include <semaphore.h>
#include <time.h> //for rand, srand
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};

queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char memory[MEMORY_SIZE]; // memory size

//--------------------------------------------------------------------------------
void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue

	node temp_node_to_queue;
	temp_node_to_queue.id = thread_id;
	temp_node_to_queue.size = size;
	myqueue.push(temp_node_to_queue);
}

//--------------------------------------------------------------------------------
void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.

	int index_memory = 0;
	while (true) 
	{
		pthread_mutex_lock(&sharedLock);
		if(!myqueue.empty())
		{
			node temp_thread_node = myqueue.front();
			myqueue.pop();

			if(temp_thread_node.size < MEMORY_SIZE - index_memory)
			{
				thread_message[temp_thread_node.id] = index_memory;
				index_memory += temp_thread_node.size;
			}
			else
			{
				thread_message[temp_thread_node.id] = -1;
			}
			sem_post(&semlist[temp_thread_node.id]);
		}
		pthread_mutex_unlock(&sharedLock);
	}
}

//--------------------------------------------------------------------------------
void * thread_function(void * id)
{
	//This function will create a random size, and call my_malloc

  	int * _id = (int*) id;
	srand (time(NULL));
  	int random_size = rand() % (MEMORY_SIZE/6) + 1; 

	pthread_mutex_lock(&sharedLock);
	if(random_size <= MEMORY_SIZE/6)
	{
		my_malloc(*_id, random_size);
		pthread_mutex_unlock(&sharedLock);

		//Block
		sem_wait(&semlist[*_id]); // down

		//Then fill the memory with 1's or give an error prompt
		if(thread_message[*_id] == -1)
		{
			printf("thread[%d]: Not enough memory\n" ,*_id);
		}
		else
		{
			for (size_t i = 0; i < random_size; i++)
			{
				//printf("%d\n", *c_id); 
				memory[thread_message[*_id]+i] = *_id + '0';
			}
		}
	}
	else
	{
		pthread_mutex_unlock(&sharedLock);
	}
		
  	return NULL;
}

//--------------------------------------------------------------------------------
void init()
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server
	pthread_mutex_unlock(&sharedLock); //unlock
}

//--------------------------------------------------------------------------------
void dump_memory()
{
	// You need to print the whole memory array here.

	cout << "Memory Dump: \n";
	for (size_t i = 0; i < MEMORY_SIZE; i++) 
	{
		cout << memory[i];
	}
}

//--------------------------------------------------------------------------------
int main (int argc, char *argv[])
 {

	//You need to create a thread ID array here

	pthread_t threads[NUM_THREADS];
    int thread_id[NUM_THREADS];

	for (size_t i = 0; i < NUM_THREADS; i++) 
	{
		pthread_t temporary;
		threads[i] = temporary;
		thread_id[i] = i;
	}

 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	for (int j = 0; j < NUM_THREADS; j++) 
	{
		pthread_create( &threads[j], NULL, thread_function, (void *) & thread_id[j]);
	}

 	//You need to join the threads
	for (size_t k = 0; k < NUM_THREADS; k++) 
	{
		pthread_join(threads[k], NULL);
	}

 	dump_memory(); // this will print out the memory

 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }