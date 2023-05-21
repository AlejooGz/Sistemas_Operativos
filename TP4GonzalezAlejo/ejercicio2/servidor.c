#include <stdio.h>
#include "interfaz.h"

int main(int argc, char** argv)
{
	printf("Soy el servidor!...\n");
	hardware();
	comunicaciones();
	return 0;
}