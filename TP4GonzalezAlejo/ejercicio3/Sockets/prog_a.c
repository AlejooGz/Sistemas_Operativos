/*
    Cliente
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>    //socket
#include <sys/un.h>    //socket
//#include <arpa/inet.h> //inet_addr

#define PATH "/tmp/DemoSocket"

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_un server;
	char message[1000] , server_reply[2000];

	//unlink(PATH);

	//Creo el socket
    	if ((sock = socket(AF_UNIX , SOCK_STREAM , 0)) < 0)
    	{
        	perror("Error al crear socket");
		exit(EXIT_FAILURE);
    	}
    	puts("Socket creado\n");

	//Prepara estructura sockaddr_un
    	server.sun_family = AF_UNIX;
    	//server.sun_path = PATH;
	strncpy(server.sun_path, PATH, strlen(PATH));

    	//Conecto al servidor
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	perror("Error al conectar con el servidor");
        	exit(EXIT_FAILURE);
    	}

    	puts("Conectado...! \n");

    	//mantengo comunicación
	while(1)
    	{
        	printf("Ingrese un mensaje: \n");
        	//scanf("%s\n", message);
		//fflush(stdin);
		//fgets(message, strlen(message), stdin);
		gets(message);

		if((strlen(message)) == 0)
		{
			return 0;
		}
		else if( send(sock , message , strlen(message) , 0) < 0)
        	{
            		puts("Error al enviar");
            		exit(EXIT_FAILURE);
        	}
    	}

    	close(sock);
    	return 0;
}
