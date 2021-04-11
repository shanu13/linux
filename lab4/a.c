#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 

#define BUFFER_SIZE 16

// circular buffer

struct podcons {
	int buffer[BUFFER_SIZE]; // main buffer to store data
	pthread_mutex_t lock;	// mutex ensuring exclusive access to buffer 
	int readpos, writepos;	 // reading , writing postions
	pthread_cond_t notempty;	// give signal when buffer is not empty 
	pthread_cond_t notfull;	// give  signal when buffer is not full 

};

// initialize buffer

void init(struct podcons *buff){
	pthread_mutex_init(&buff->lock, NULL);
	pthread_cond_init(&buff->notempty, NULL);
	pthread_cond_init(&buff->notfull, NULL);
	buff->readpos = 0;
	buff->writepos = 0;
}

// stores value to buffer

void put(struct podcons *buff, int data)
{
	pthread_mutex_lock(&buff->lock);
	// Wait until buffer is not full 
	while ((buff->writepos + 1) % BUFFER_SIZE == buff->readpos) {
		pthread_cond_wait(&buff->notfull, &buff->lock);
		// pthread_cond_wait reacquired buff->lock before returning 
	}
	// Write the data and advance write pointer 
	buff->buffer[buff->writepos] = data;
	buff->writepos++;
	if (buff->writepos >= BUFFER_SIZE)
		buff->writepos = 0;
	// give signal that the buffer is now not empty 
	pthread_cond_signal(&buff->notempty);
	pthread_mutex_unlock(&buff->lock);
}

// Read and remove the data from the buffer 

int get(struct podcons *buff)
{
	int data;
	pthread_mutex_lock(&buff->lock);
	// Wait until buffer is not empty 
	while (buff->writepos == buff->readpos) {
		pthread_cond_wait(&buff->notempty, &buff->lock);
	}
	 //Read the data and advance read pointer 
	data = buff->buffer[buff->readpos];
	buff->readpos++;
	if (buff->readpos >= BUFFER_SIZE)
		buff->readpos = 0;
	// give signal that the buffer is now not full
	pthread_cond_signal(&buff->notfull);
	pthread_mutex_unlock(&buff->lock);
	return data;
}

// A test program: one thread inserts integers from 1 to 10000,
  // the other reads them and prints them.

#define over (-1)

struct podcons buffer;

void *producer(void *data)
{
	int i;
	for (i = 0; i < 10000; i++) {
		printf("%d --->\n", i);
		put(&buffer, i);
	}
	put(&buffer, over);
	return NULL;
}

void *consumer(void *data)
{
	int d;
	while (1) 
	{
		d = get(&buffer);
		if (d == over)
		{
		    break;
		}
		printf("---> %d\n", d);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thread_a,thread_b;
	void *retval;
	init(&buffer);

 // cretaing the threads

	if((pthread_create(&thread_a,NULL,producer,0))!=0 || (pthread_create(&thread_b,NULL,consumer,0))!=0) {
		fprintf(stderr, "thread creation failed\n");
		return 0;
	} 
	// wait until producer and consumer finish
	pthread_join(thread_a,&retval);
	pthread_join(thread_b,&retval);
	return 0;
}