#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int sumArray(int*arr, int begin, int end);
void* getSharedMemory(int size);

int main(int argc, char *args[]){

	int size = atoi(args[0]);
	int begin = atoi(args[1]);
	int end = atoi(args[2]);
	
	int *arr = (int*)getSharedMemory(size);

	int respuesta = sumArray(arr, begin, end);

	printf("Respuesta de Proceso %d: %d\n", getpid(),respuesta);
	return respuesta;
}

int sumArray(int*arr, int begin, int end){
	int i;
	int suma = 0;
	for(i = begin; i<end; i++){
		suma += arr[i];
	}
	return suma;
}

void* getSharedMemory(int size){
	key_t key;
	int shmid;

	key = 12345;
	shmid = shmget(key, size, 0644 | IPC_CREAT);
	return shmat(shmid, (void *)0, 0);
}