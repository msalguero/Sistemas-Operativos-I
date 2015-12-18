#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entrada
{
	short base;
	short limite;
};

using namespace std;

void inicializarTablaSegmentos(struct Entrada*tabla);

int calcularDireccionFisica(struct Entrada* tabla, int cantEntradas, int direccion){
	short segmento;
	short offset;
	memcpy(&segmento, &direccion, 2);
	memcpy(&segmento,((short*) &direccion + 1), 2);
	if(segmento > cantEntradas)
		return -1;
	int base = tabla[segmento].base;
	int limite = tabla[segmento].limite;

	if(offset > limite)
		return -1;
	return base + limite;
}

int main(){
	//int tamano = sizeof(struct Entrada);
	//printf("%d\n", tamano);
	int cantidadEntradas = 4;
	struct Entrada tabla[cantidadEntradas];
	inicializarTablaSegmentos(tabla);


	int direccion;
	printf("Ingresar Direccion\n");
	scanf("%d", &direccion);


	int direccionFisica = calcularDireccionFisica(tabla, cantidadEntradas, direccion);
	if(direccionFisica != -1){
		printf("Direccion Fisica: %d\n", direccionFisica);
	}else{
		printf("Direccion no valida\n");
	}
}

void inicializarTablaSegmentos(struct Entrada* tabla){
	tabla[0].base = 1000;
	tabla[0].limite = 200;
	tabla[1].base = 1200;
	tabla[1].limite = 500;
	tabla[2].base = 1700;
	tabla[2].limite = 300;
	tabla[3].base = 2000;
	tabla[3].limite = 200;
}