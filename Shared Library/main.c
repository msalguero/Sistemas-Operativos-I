#include <stdio.h>
#include "suma.h"
 
int main(void)
{
    puts("This is a shared library test...");
    int answer = suma(1, 2);
    printf("%d\n", answer);
    return 0;
}
//compile .so
// gcc -c -Wall -Werror -fpic suma.c
//gcc -shared -o libsuma.so suma.o
//compile main
// gcc -L/home/rafa/Documents/Siso/Shared\ Library -Wl,-rpath=/home/rafa/Documents/Siso/Shared\ Library -Wall -o main main.c -lsuma
