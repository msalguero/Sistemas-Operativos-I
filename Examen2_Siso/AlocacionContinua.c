#include <stdio.h>

int calcularDireccionFisica(int base, int limite, int direccion){
	if(direccion > limite){
		return -1;
	}
	return base + direccion;
}

int main(){
	int base;
	printf("Ingresar Base\n");
	scanf("%d", &base);

	int limite;
	printf("Ingresar Limite\n");
	scanf("%d", &limite);

	int direccion;
	printf("Ingresar Direccion\n");
	scanf("%d", &direccion);


	int direccionFisica = calcularDireccionFisica(base, limite, direccion);
	if(direccionFisica != -1){
		printf("Direccion Fisica: %d\n", direccionFisica);
	}else{
		printf("Direccion no valida\n");
	}
}

