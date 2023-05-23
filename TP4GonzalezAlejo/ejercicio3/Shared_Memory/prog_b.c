/*
 * Servidor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>								//shm_open y mmap
#include <sys/stat.h>       /* Constantes de modo */ 	//shm_open
#include <fcntl.h>           	/* Constantes O_* */		//shm_open
#include <unistd.h>		//ftruencate
#include <sys/types.h>	//ftruencate
#include <semaphore.h>

#define MAX_MSG 200

int main(void)
{
	int shm_fd;
	char *data;
	sem_t *sem1, *sem2;

	sem1 = sem_open("/sem1_shared", O_CREAT | O_RDWR, 0777, 1);
	sem2 = sem_open("/sem2_shared", O_CREAT | O_RDWR, 0777, 0);

	shm_fd = shm_open("/shm_ej3", O_RDWR | O_CREAT, 0777);
	if(shm_fd == -1)
	{
		printf("Error al crear la zona compartida de memoria\n");
		exit(-1);
	}

	if((ftruncate(shm_fd, sizeof(char) * MAX_MSG)) == -1)
	{
		printf("Error al truncar el tamaño de la memoria compartida\n");
		exit(-1);
	}

	data = (char *)mmap(NULL, sizeof(char) * MAX_MSG, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(data == MAP_FAILED)
	{
		printf("Error al mapear\n");
		exit(-1);
	}

	while(1)
	{
		sem_wait(sem2);
		printf("data = %s\n", data);
		sem_post(sem1);
	}

	sem_close(sem1);
	sem_close(sem2);

	return 0;
}
