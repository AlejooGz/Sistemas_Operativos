/*
* Cliente
*/

#include "commons.h"
#define SIZE 8192

int main()
{  
   	char mensaje[SIZE];
	
	mq_atributos.mq_flags = 0;
  	mq_atributos.mq_maxmsg = 1;
  	mq_atributos.mq_msgsize = sizeof(char)*SIZE; 
   	mq_atributos.mq_curmsgs = 1;
	
	mqd = mq_open("/mq_ejercicio3", O_CREAT | O_WRONLY, 0666, &mq_atributos);		

	if(mqd == (mqd_t)-1)	//(mqd_t)
	{
		perror("Error en mq_open");
		exit (-1);
	}

	while(1)
	{
		printf("Ingrese un mensaje:\n");
		//fgets(mensaje, sizeof(char) * SIZE, stdin);
		gets(mensaje);

		if(strlen(mensaje) == 0)
			return 0;

  		if(mq_send(mqd, mensaje, mq_atributos.mq_msgsize, 1) == -1)
			perror("Error en mq_send");
	}

	mq_close(mqd);
   	mq_unlink("/mq_ejercicio3");

   	return 0;
}