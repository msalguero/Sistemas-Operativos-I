#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];

void *producer(void *param);
void *consumer(void *param);
void create_producers();
void create_consumers();
void init();

void initialize_buffer(){
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = -1;
	}
}


int insert_item(buffer_item item) {
	
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if(buffer[i] == -1){
			buffer[i] = item;
			printf ("producer produced %d\n", item);
			return 0;
		}
	}
	return -1;
}

int remove_item(buffer_item *item) {
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if(buffer[i] != -1){
			*item = buffer[i];
			buffer[i] = -1;
			printf("consumer consumed %d\n", *item);
			return 0;
		}
	}
	return -1;
}

pthread_mutex_t mutex;
sem_t sem_available;
sem_t sem_produced;

int main(int argc, char*argv[]) {
	initialize_buffer();
	init();
	create_producers();
	create_consumers();
	/* 2. Initialize buffer, mutex, semaphores, and other global vars */
	/* 3. Create producer thread(s) */
	/* 4. Create consumer thread(s) */
	/* 5. Sleep */
	/* 6. Release resources, e.g. destroy mutex and semaphores */
	/* 7. Exit */
	sleep (60);
	pthread_mutex_destroy(mutex);
}

void create_producers(){
	pthread_t tid;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	
	pthread_create(&tid, &attr, producer, NULL);
}

void create_consumers(){
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, consumer, NULL);
}

void *producer(void *param) {
	unsigned int seed = time(NULL);
	buffer_item rand;
	int seconds;
	while (1) {
		seconds = rand_r(&seed) % 10;
		sleep(seconds);

		sem_wait(&sem_available);
		pthread_mutex_lock(&mutex);
		
		rand = rand_r(&seed) % 100;
		if (insert_item(rand) < 0)
			printf("Error producing item\n");

		pthread_mutex_unlock(&mutex);
		sem_post(&sem_produced);
	}
}

void *consumer(void *param) {
	unsigned int seed = time(NULL);
	buffer_item rand;
	int seconds;
	while (1) {
		seconds = rand_r(&seed) % 60;
		sleep (seconds);

		sem_wait(&sem_produced);
		pthread_mutex_lock(&mutex);

		rand = rand_r(&seed) % 100;
		if (remove_item(&rand) < 0)
			printf("Error consuming item\n");

		pthread_mutex_unlock(&mutex);
		sem_post(&sem_available);
	}
}

void init(){
	sem_init(&sem_available, 0, BUFFER_SIZE);
	sem_init(&sem_produced, 0, 0);
	pthread_mutex_init(&mutex, NULL);
}