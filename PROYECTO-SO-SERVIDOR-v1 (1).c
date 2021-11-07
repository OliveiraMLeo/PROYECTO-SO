#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>

int log_in (char nombre[20], char contra[20])
{
	MYSQL *conn;
	int err;
	
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	//Creamos una conexion
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf ("Error al crear la conexion \n: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//Inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root","mysql","BBDD_JUEGO",0,NULL,0);
	if (conn == NULL)
	{
		printf ("Error al inicialiazar la conexion \n: %u %s", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//CONSULTA SQL
	char consulta [512];
	sprintf(consulta, "SELECT * FROM (Jugador) WHERE NOMBRE = '%s' AND CONTRASEÑA = '%s'",nombre, contra);
	err = mysql_query (conn, consulta);
	
	if (err != 0)
	{
		printf ("Error al consultar datos de la base %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//Recogemos el resultado de la consulta.
	resultado = mysql_store_result(conn);
	
	row = mysql_fetch_row(resultado);
	
	int encontrado = 0;
	if (row == NULL)
	{
		printf ("No se han obtenido datos en la consulta \n");
	}
	else
	{
		encontrado = 1;
	}
	
	//Cerrar la conexion
	mysql_close(conn);
	return encontrado;
	exit(0);
}

int SumaPuntos (char nombre[20])
{
	MYSQL *conn;
	int err;
	
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	//Creamos una conexion
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf ("Error al crear la conexion \n: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//Inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root","mysql","BBDD_JUEGO",0,NULL,0);
	if (conn == NULL)
	{
		printf ("Error al inicialiazar la conexion \n: %u %s", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//CONSULTA SQL
	char consulta [512];
	sprintf(consulta, "SELECT sum(PUNTUACION) FROM (Puente,Jugador,Partida) WHERE NOMBRE = '%s' AND GANADOR = '%s' AND NOMBRE = IDj and ID = IDp",nombre, nombre);
	err = mysql_query (conn, consulta);
	
	if (err != 0)
	{
		printf ("Error al consultar datos de la base %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	//Recogemos el resultado de la consulta.
	resultado = mysql_store_result(conn);
	
	row = mysql_fetch_row(resultado);
	int suma;
	if (row == NULL)
	{
		printf ("No se han obtenido datos en la consulta \n");
	}
	else
	{
		suma = atoi(row[0]);
	}
	
	//Cerrar la conexion
	mysql_close(conn);
	return suma;
	exit(0);
}


int main(int argc, char *argv[]) 
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion [512];
	char respuesta [512];
	
	if ((sock_listen = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		printf ("Error creando socket");
	
	memset(&serv_adr, 0, sizeof(serv_adr)); //Inicializa a zero serv_adr
	serv_adr.sin_family = AF_INET;
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //cualquier IP
	serv_adr.sin_port = htons(9080);
	
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0 )
	    printf("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el listen");
	
	//Bucle infinito
	for (;;)
	{
		printf("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		
		//Bucle de atencion
		int terminar = 0;
		while (terminar == 0)
		{
			
			ret = read (sock_conn, peticion, sizeof(peticion));
			printf("Recibido\n");
			
			peticion[ret] = '\0';
			
			printf("Peticion: %s\n", peticion);
			
			char *p = strtok(peticion, "/");
			int codigo = atoi(p);
			char nombre[20];
			char contra[20];
			
			if (codigo == 0)
			{ terminar = 1; }
			else if (codigo == 1)
			{ 
				p = strtok(NULL, "/");
				strcpy(nombre, p);
				p = strtok(NULL, "/");
				strcpy(contra, p);
				
				int encontrado = log_in(nombre, contra);
				if (encontrado == 1)
				{ strcpy(respuesta, "SI"); }
				else 
				{ strcpy(respuesta, "NO"); }
			}
			else if (codigo == 2)
			{
				int suma = SumaPuntos(nombre);
				respuesta[0] = suma;
			}
			
			if (codigo != 0)
			{ 
				printf("Respuesta: %s\n", respuesta);
				write(sock_conn, respuesta, strlen(respuesta)); 
			}
		
		}
		
		close(sock_conn);
	}
}




