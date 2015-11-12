#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>

void printArray(int*arr, int size);
void* getSharedMemory(int size);
char* IntToCharPointer(int intToConvert);
int shmid;

int main(){

	int size = 0;
	int cantidadProcesos = 0;

	printf("Ingresar size del arreglo: ");
	scanf("%d", &size);

	printf("Ingresar cantidad de procesos Sumadores: ");
	scanf("%d", &cantidadProcesos);

	int *arr = (int*)getSharedMemory(size); 

	int i;
	for(i = 0; i < size; i++){
		printf("Ingresar arreglo en la posicion %d: \n", i);
		scanf("%d", &arr[i]);
	}

	printf("Resultado de Procesos\n");

	int valorMaximo = 0;
	int procesoDeMayorValor = -1;
	for(i = 0; i<cantidadProcesos; i++){
		int resultadoProcesoActual = 0;
		int begin = (size/cantidadProcesos)*i;
		int end = begin + (size/cantidadProcesos);
		int pid = fork();
		if(pid == 0)
			execl("./Sumador", IntToCharPointer(size), IntToCharPointer(begin), IntToCharPointer(end), (char *)0);
		else{
			waitpid (pid, &resultadoProcesoActual, 0);
			if( WEXITSTATUS(resultadoProcesoActual) > valorMaximo){
				procesoDeMayorValor = pid;
				valorMaximo = WEXITSTATUS(resultadoProcesoActual);
			}
		}
	}

	printf("Proceso de Mayor Valor: %d\nCon la cantidad: %d\n", procesoDeMayorValor, valorMaximo);

}

void* getSharedMemory(int size){
	key_t key;
	char* data;
	key = 12345;
	shmid = shmget(key, size, 0644 | IPC_CREAT);
	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1))
    	perror("shmat");
	return data;
}

void* DeleteSharedMemory(){
	shmctl(shmid, IPC_RMID, NULL);
}

char* IntToCharPointer(int intToConvert){
	char* str = (char*)malloc(4);
	sprintf( str, "%d", intToConvert );
	return str;
}
