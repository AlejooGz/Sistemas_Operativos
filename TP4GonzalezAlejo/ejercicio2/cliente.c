#include <stdio.h>
#include "interfaz.h"

int main(int argc, char** argv)
{
	printf("Soy el cliente!...\n");
	hardware();
	comunicaciones();
	return 0;
}