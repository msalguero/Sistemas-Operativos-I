#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct FibonacciParameters{
	int number;
	int answer;
};

void* Fibonacci(void* params);

int main(){

	int n = 0;
	printf("Ingresar numero a calcular: ");
	scanf("%d", &n);

	struct FibonacciParameters params = {n, 0};

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, Fibonacci, &params);

	pthread_join(tid, 0);

	printf("La respuesta es: %d\n", params.answer);
}

void* Fibonacci(void* params){
	struct FibonacciParameters* parameters = (struct FibonacciParameters*)params;
	int n = parameters->number;
	if(n == 0 || n == 1){
		parameters->answer = 1;
		pthread_exit(0);
	}

	struct FibonacciParameters paramsFirst = {n - 1, 0};
	struct FibonacciParameters paramsSecond = {n - 2, 0};

	pthread_t tidFirst;
	pthread_attr_t attrFirst;
	pthread_attr_init(&attrFirst);
	pthread_create(&tidFirst, &attrFirst, Fibonacci, &paramsFirst);
	pthread_join(tidFirst, 0);

	pthread_t tidSecond;
	pthread_attr_t attrSecond;
	pthread_attr_init(&attrSecond);
	pthread_create(&tidSecond, &attrSecond, Fibonacci, &paramsSecond);
	pthread_join(tidSecond, 0);

	parameters->answer = paramsFirst.answer + paramsSecond.answer;
}