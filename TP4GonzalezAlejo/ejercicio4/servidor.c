/*
 * Sistemas Operativos - UNPSJB
*/
 
#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>     //close
#include <arpa/inet.h>  //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>   //FD_SET, FD_ISSET, FD_ZERO macros
 
#define TRUE   1
#define FALSE  0
#define PORT 8888    //puerto escucha del servidor

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int socket_master, addrlen, nuevo_socket, socket_cliente[30], max_clientes=30, actividad, i, leido, sd;
	 int max_sd;
    struct sockaddr_in direccion;
     
    char buffer[1025];  //data buffer of 1K
     
    //set de descriptores de sockets
    fd_set fds_lectura;
     
    //mensaje
    char *mensaje = "ECHO Daemon v1.0 \r\n";
 
    for (i = 0; i < max_clientes; i++) 
    {
        socket_cliente[i] = 0;
    }
     
    //crea un socket y este es asignado a socket_master
    if( (socket_master = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("Llamada socket falló");
        exit(EXIT_FAILURE);
    }
 
    //configura las opciones del socket
    if( setsockopt(socket_master, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("Llamada setsockopt falló");
        exit(EXIT_FAILURE);
    }
 

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PORT);
     
    //vincula el socket al puerto
    if (bind(socket_master, (struct sockaddr *)&direccion, sizeof(direccion))<0) 
    {
        perror("Llamada bind falló");
        exit(EXIT_FAILURE);
    }
	 printf("Escuchando en puerto %d \n", PORT);
	
    //espera las conexiones que van a llegar
	 if (listen(socket_master, 3) < 0)
    {
        perror("Llamada listen falló");
        exit(EXIT_FAILURE);
    }
     
    //obtiene el tamaño de la direccion entrante
    addrlen = sizeof(direccion);
    puts("Esperando conexiones entrantes ...");
    
	 while(TRUE) 
    {
        //borra el descriptor de archivo
        FD_ZERO(&fds_lectura);
 
	     //setea el socket
        FD_SET(socket_master, &fds_lectura);
        max_sd = socket_master;
		
		  //agrega sockets de cliente al conjunto
        for (i = 0 ; i < max_clientes ; i++) 
        {
        		//descriptor de socket
				sd = socket_cliente[i];
            
				//si hay descriptor de socket valido entonces se añade a la lista de lectura
				if (sd > 0)
					FD_SET(sd, &fds_lectura);
            
            	//maximo numero de descriptores de socket
            	if (sd > max_sd)
						max_sd = sd;
        }
 
        //monitorea los descriptores de archivos de lectura
        actividad = select( max_sd + 1 , &fds_lectura , NULL , NULL , NULL);
   
        if ((actividad < 0) && (errno!=EINTR)) 
        {
            printf("Llamada a select falló");
        }
         
        //verifica que no haya algo pendiente en el socket
        if (FD_ISSET(socket_master, &fds_lectura)) 
        {
            if ((nuevo_socket = accept(socket_master, (struct sockaddr *)&direccion, (socklen_t*)&addrlen))<0)
            {
                perror("Llamada accept falló");
                exit(EXIT_FAILURE);
            }

            printf("Nueva conexion , file descriptor de socket es %d , ip es : %s , puerto : %d \n" , nuevo_socket , inet_ntoa(direccion.sin_addr) , ntohs(direccion.sin_port));
       
            //el socket envia un mensaje al cliente
            if( send(nuevo_socket, mensaje, strlen(mensaje), 0) != strlen(mensaje) ) 
                perror("Llamada send falló");
             
            puts("Mensaje enviado exitosamente");
             
            //añade el socket a la lista de sockets con clientes conectados
            for (i = 0; i < max_clientes; i++) 
            {
                
					if( socket_cliente[i] == 0 )
               {
               	socket_cliente[i] = nuevo_socket;
                  printf("Añadiendo a lista de sockets como %d\n" , i);
					
						break;
               }
            }
        }
         
        //Si hay alguna otra operacion en algun otro socket
        for (i = 0; i < max_clientes; i++) 
        {
            sd = socket_cliente[i];
             
            if (FD_ISSET(sd , &fds_lectura)) 
            {
                
					if ((leido = read(sd ,buffer, 1024)) == 0)
                {
                    //Alguno se desconetó, obtengo detalles y muestro
                    getpeername(sd , (struct sockaddr*)&direccion , (socklen_t*)&addrlen);
                    printf("Host desconectado , ip %s , puerto %d \n" , inet_ntoa(direccion.sin_addr) , ntohs(direccion.sin_port));
                     
                    //Cierro socket y marco como disponible en lista
                    close( sd );
                    socket_cliente[i] = 0;
                }
                 
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[leido] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
     
    return 0;
}