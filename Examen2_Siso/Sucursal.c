#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

static int N = 4;
int M = 10;
int X = 50;
int Y = 2;
int Z = 5;

int cantPersonas;
sem_t sem_cajeros_disponibles;
sem_t sem_personas_en_fila;
pthread_mutex_t mutex;
bool cajerosMaximo;

void *persona(void *param);
void *cajero(void *param);
void *generarPersona(void *param);
void crearCajero();

int main(){
	cajerosMaximo = false;;
	cantPersonas = 0;
	sem_init(&sem_cajeros_disponibles, 0, N);
	sem_init(&sem_personas_en_fila, 0, 0);
	pthread_mutex_init(&mutex, NULL);
	printf("Atendiendo con %d Cajeros\n", N/2);
	int i;
	for(i = 0; i < N/2; i++){
		crearCajero();
	}

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, generarPersona, NULL);
	sleep (120);
}

void* generarPersona(void *param){
	unsigned int seed = time(NULL);
	int seconds;

	while (1) {
		int value = rand_r(&seed);
	    int length = Z - Y + 1;
	    seconds =  Y + (value % length);
		//seconds = rand_r(&seed) % 10;
		sleep(seconds);

		pthread_t tid;
		pthread_attr_t attr;
		
		pthread_attr_init(&attr);
		
		pthread_create(&tid, &attr, persona, NULL);
	}
}

void crearCajero(){
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, cajero, NULL);
}

void *persona(void *param) {
	unsigned int seed = time(NULL);
	int seconds;
	if(cantPersonas < M){
		sem_post(&sem_personas_en_fila);
		printf("Persona entro a fila\n");
		cantPersonas++;
	}else{
		printf("Persona se fue\n");
	}
	
	if((cantPersonas *100) / M > 50 && !cajerosMaximo){
		cajerosMaximo = true;
		int i;
		for(i = 0; i < N/2; i++){
			crearCajero();
		}
		printf("Cantidad de personas en fila: %d\n", cantPersonas);
		printf("Atendiendo con %d Cajeros\n", N);
	}
}

void *cajero(void *param) {
	unsigned int seed = time(NULL);
	int seconds;
	while (1) {
		sem_wait(&sem_cajeros_disponibles);
		sem_wait(&sem_personas_en_fila);
		pthread_mutex_lock(&mutex);
		cantPersonas--;
		pthread_mutex_unlock(&mutex);
		int value = rand_r(&seed);
	    int length = X - 1 + 1;
	    seconds =  1 + (value % length);

	    printf("Cajero Atendiendo nevo Cliente\n");
		sleep (seconds);
		printf("Cajero termino de atender en %d segundos\n", seconds);
		
		sem_post(&sem_cajeros_disponibles);
	}
}