/*
    Servidor
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "/tmp/DemoSocket"
#define MAX_MSG 2000
int main(int argc , char *argv[])
{
    	int server_sock , client_sock , c , read_size;
    	struct sockaddr_un server , client;
    	char client_message[MAX_MSG];

	unlink(PATH);

    	//Crea socket
    	server_sock = socket(AF_UNIX , SOCK_STREAM , 0);
    	if (server_sock < 0)
    	{
        	perror("No se pudo crear socket");
		exit(EXIT_FAILURE);
    	}
    	puts("Socket creado");

    	//Prepara estructura sockaddr_un
    	server.sun_family = AF_UNIX;
	strncpy(server.sun_path, PATH, strlen(PATH));

    	//Bindeo
    	if( bind(server_sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	//muestro mensaje de error
        	perror("Error al realizar bind");
		exit(EXIT_FAILURE);
    	}

    	puts("Bind hecho");

    	//Escucho
    	if(listen(server_sock , 3) < 0)
	{
        	perror("Error al realizar listen");
		exit(EXIT_FAILURE);
    	}

    	//espero conexiones
    	puts("Esperando por conexiones entrantes...");
    	c = sizeof(struct sockaddr_un);

    	//acepto conexines de clientes
    	client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c);
    	if (client_sock < 0)
    	{
        	perror("Error al realizar accept");
        	exit(EXIT_FAILURE);
    	}
   	puts("Conexión aceptada");

    	//Recibo un mensaje de un cliente
    	while( (read_size = recv(client_sock , client_message , MAX_MSG , 0)) > 0 )
        	printf("Recibí de un cliente: %s\n", client_message);	//muestro el mensaje por la salida estandar

    	if(read_size == 0)
    	{
        	printf("Cliente desconectado\n");
        	fflush(stdout);
    	}
    	else if(read_size < 0)
    	{
       		perror("Error al hacer el recive");
		exit(EXIT_FAILURE);
    	}
	close(client_sock);
    	return 0;
}
