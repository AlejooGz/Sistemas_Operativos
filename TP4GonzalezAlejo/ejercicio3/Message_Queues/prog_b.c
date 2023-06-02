/*
* Servidor
*/

#define SIZE 8192
#include "commons.h"

int main()
{
   	char  msg[SIZE];

   	mq_atributos.mq_flags = 0;
   	mq_atributos.mq_maxmsg = 1;
   	mq_atributos.mq_msgsize = sizeof(char)*SIZE;
   	mq_atributos.mq_curmsgs = 1;
   
   	mqd = mq_open("/mq_ejercicio3", O_CREAT | O_RDONLY, 0666, &mq_atributos);

   	if(mqd == (mqd_t) -1)
	{
      		perror("Error en mq_open");
      		exit (-1);
   	}

	while(1)
	{
		if(mq_receive(mqd, msg, SIZE,NULL) == -1)
      			perror("Error en mq_receive");

		if(strlen(msg) == 0)
			return 0;

		printf("%s\n", msg);
	}

   	mq_close(mqd);
   	//mq_unlink("/mq_ejercicio3");

   	return 0;
}