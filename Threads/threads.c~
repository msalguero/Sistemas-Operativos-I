#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct SortParameters{
	int *arr;
	int begin;
	int end;
};
void buildRandomArray(int *arr, int size);
void printArray(int*arr, int size);
void* sortArrayBySegments(void* params);
void merge(int* arr, int leftBegin, int leftEnd, int rightBegin, int rightEnd);
void mergeArray(int* arr, int size, int cantidadHilos);
void multiThreadedSort(int *arr, int size, int cantidadHilos, pthread_t** threadArr);

int main(){
	int size = 0;
	int cantidadHilos = 0;
	printf("Ingresar size del arreglo: ");
	scanf("%d", &size);
	printf("Ingresar cantidad de Hilos: ");
	scanf("%d", &cantidadHilos);
	int *arr = (int*)malloc(sizeof(int)*size); 
	pthread_t **threadArr = (pthread_t**)malloc(sizeof(pthread_t*)*cantidadHilos);

	buildRandomArray(arr, size);

	clock_t start = clock(), diff;

	multiThreadedSort(arr, size, cantidadHilos, threadArr);
	
	mergeArray(arr, size, cantidadHilos);

	diff = clock() - start;

	printf("Despues de sortear\n");
	printArray(arr, size);

	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("\n\nTiempo tomado %d segundos %d milisegundos", msec/1000, msec%1000);

}

void printArray(int*arr, int size){
	int i;
	for(i = 0; i<size; i++){
		printf("%d ", arr[i]);
	}
}

void* sortArrayBySegments(void* params){
		struct SortParameters* parameters = (struct SortParameters*)params;
		int *arreglo = parameters->arr;
		int i;
		for(i = parameters->begin; i<parameters->end; i++){
			int j;
			for(j = parameters->begin; j<parameters->end; j++){
				if(arreglo[i] > arreglo[j]){
					int temp = arreglo[i];
					arreglo[i] = arreglo[j];
					arreglo[j] = temp;
				}
			}
		}
		free(parameters);
		pthread_exit(0);
}

void mergeArray(int* arr, int size, int cantidadHilos){
	int i;
	while(cantidadHilos > 1){
		for(i = 0; i < cantidadHilos; i = i + 2){
			int leftBegin = (size/cantidadHilos)*(i);
			int leftEnd = leftBegin + (size/cantidadHilos);
			int rightBegin = (size/cantidadHilos)*(i + 1);
			int rightEnd = rightBegin + (size/cantidadHilos);
			merge(arr, leftBegin, leftEnd, rightBegin, rightEnd);
		}
		cantidadHilos /= 2;
	}
}

void merge(int* arr, int leftBegin, int leftEnd, int rightBegin, int rightEnd){
	int size = (leftEnd - leftBegin) + (rightEnd - rightBegin);
	int arrayPointer = leftBegin;
	int i = 0;
	int *tempArray = (int*)malloc(sizeof(int)*size);

	while(leftEnd - leftBegin > 0 &&  rightEnd - rightBegin > 0 ){
		if (arr[leftBegin] >= arr[rightBegin]){
			tempArray[i] = arr[leftBegin++];
		}
		else{
			tempArray[i] = arr[rightBegin++];
		}
		i++;
	}
	if (leftEnd - leftBegin > 0 ){
		while(leftEnd - leftBegin > 0){
			tempArray[i] = arr[leftBegin++];
			i++;
		}
	}
	if (rightEnd - rightBegin > 0 ){
		while(rightEnd - rightBegin > 0){
			tempArray[i] = arr[rightBegin++];
			i++;
		}
	}
	
	for(i = 0; i < size; i++){
  		arr[arrayPointer++] = tempArray[i];
	}

	free(tempArray);	
}

void multiThreadedSort(int *arr, int size, int cantidadHilos, pthread_t** threadArr){
	int i;
	for(i = 0; i < cantidadHilos; i++){
		struct SortParameters *params = (struct SortParameters*)malloc(sizeof(struct SortParameters));
		params->arr = arr;
		params->begin = (size/cantidadHilos)*i;
		params->end = params->begin + (size/cantidadHilos);
			
		pthread_t tid;
		pthread_attr_t attr;
		
		threadArr[i] = &tid;

		pthread_attr_init(&attr);

		pthread_create(&tid, &attr, sortArrayBySegments, params);
	}
	for(i = 0; i < cantidadHilos; i++){
		pthread_join(*threadArr[i], 0);
	}
}

void buildRandomArray(int *arr, int size){
	int i;
	for(i = 0; i<size; i++){
		arr[i] = rand() % 1000 + 1;
	}
}
