/*
* Cliente
*/

#include "commons.h"


int main()
{  
   	char *mensaje;
	
	mq_atributos.mq_flags = 0;
  	mq_atributos.mq_maxmsg = 1;
  	mq_atributos.mq_msgsize = sizeof(char)*strlen(mensaje); 
   	mq_atributos.mq_curmsgs = 1;
	
	mqd = mq_open("/mq_ejercicio3", O_CREAT | O_RDWR, 0666, &mq_atributos);		

	if(mqd == -1)	//(mqd_t)
	{
		perror("Error en mq_open");
		exit (-1);
	}

	printf("Ingrese un mensaje:\n");
	gets(mensaje);

  	if(mq_send(mqd, mensaje, mq_atributos.mq_msgsize, 1) == -1)
		perror("Error en mq_send");

	mq_close(mqd);
   	mq_unlink("/mq_ejercicio3");

   	return 0;
}