/**
 * \file sock-lib.c
 * \brief contiene las funciones simplificadas de manejo de sockets para usar en Informática I
 * \author Alejandro Furfaro
 * \date 2018.09.10
 * \author Oscar Paniagua
 * \date 2022.12.12
 */

#include "sock-lib.h"

/**
 * \fn int conectar (char *hostname, int port)
 * \brief Única función del lado cliente, que crea un socket y conecta a un server que le llega como argumento.
 * \details Se crea un socket y luego se invoca a la función connect para iniciar conexión con el nodo remoto en el
 * cual debe estar ejecutando un programa servidor escuchando conexiones por el mismo port al ue inicia la conexión.
 * El port si es 0 se utiliza el valor por default
 * \return Devuelve un entero con el file descriptor de socket
 * \author Alejandro Furfaro
 * \date 2018.09.10
 * \author Oscar Paniagua
 * \date 2022.12.12
 */
int conectar(char *hostname, int port, int debug)
{
	int sockfd;
	struct hostent *he;			   /* Se utiliza para convertir el nombre del host a su dirección IP */
	struct sockaddr_in their_addr; /* dirección del server donde se conectará */

	// convertimos el nombre del host a su dirección IP
	if ((he = gethostbyname(hostname)) == NULL)
	{
		herror("Error en Nombre de Host");
		exit(1);
	}

	/* Creamos el socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error en creación de socket");
		exit(1);
	}
	if (debug)
		fprintf(stderr, "debug:: conectar() socket()=%d\t\t..........OK\n", sockfd);

	if (debug)
		fprintf(stderr, "debug:: conectar() dst port()=%d\t..........OK\n", port);

	port = (port == 0) ? htons(PORT) : htons(port);

	/* Establecemos their_addr con la direccion del server */
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = port;
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);

	/* Intentamos conectarnos con el servidor */
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("conectar():: error tratando de conectar al server");
		exit(1);
	}
	if (debug)
		fprintf(stderr, "debug:: conectar() connect()\t\t..........OK\n");

	return sockfd;
}

/**
 * \fn int open_conection ( struct sockaddr_in * my_addr)
 * \brief Primer función del lado server, que crea un socket lo enlaza con un número de port que es el definido en el header de la
 * biblioteca, el  3490, y define cuntos buffers de conexiones armacenará durante la aceptación de al conexión, y pasa a escuchar.
 * \details Se crea un socket y luego se invoca a la función bind para indicarle al SIstema Operativo por que port escuchará conexiones.
 * Utilizará la ip default invocando a una macro definida en la biblioteca de sockets del sistema operativo. A posteriori se pondrá a escuchar
 * conexiones por el port declarado al Sistema Operativo, definiendo previamente la cantidad de pedidos de conexión remotos que buffereará
 * mientras eventualmente esté iniciando el intercambio con el cliente que pide inicio de una conexión
 * \arg[in] puntero a estrictura sockaddr_in que completará antes de invocar a bind () con el número de port y la ip por la que escuchará
 * conexiones a través del sockeet.
 * return Devuelve un entero con el file descriptor de socket
 * \author Alejandro Furfaro
 * \date 2018.09.10
 */

int abrir_conexion(int port, int backlog, int debug)
{
	int sockaux; /*socket auxiliar*/
	int aux;	 /*variable auxiliar*/
	struct sockaddr_in my_addr;

	/*Crea un socket y verifica si hubo algún error*/
	if ((sockaux = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Error en función socket(). Código de error %s\n", strerror(sockaux));
		return -1;
	}
	if (debug)
		fprintf(stderr, "debug:: abrir_conexion() socket()=%d\t..........OK\n", sockaux);

	// reuso por timeout inmediato
	int optval = 1;
	if (setsockopt(sockaux, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		fprintf(stderr, "Error en función setsockopt()\n");
		return -1;
	}
	if (debug)
		fprintf(stderr, "debug:: abrir_conexion() setsockopt()\t..........OK\n");

	/* Asignamos valores a la estructura my_addr */

	if (port == 0)
		port = PORT;

	my_addr.sin_family = AF_INET;		  /* familia de sockets INET para UNIX*/
	my_addr.sin_port = htons(port);		  /* convierte el entero formato PC a entero formato network*/
	my_addr.sin_addr.s_addr = INADDR_ANY; /* automaticamente usa la IP local */
	bzero(&(my_addr.sin_zero), 8);		  /* rellena con ceros el resto de la estructura */

	/* Con la estructura sockaddr_in completa, se declara en el Sistema que este proceso escuchará pedidos por la IP y el port definidos*/
	if ((aux = bind(sockaux, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))) == -1)
	{
		fprintf(stderr, "Error en función bind. Código de error %s\n", strerror(aux));
		return -1;
	}
	if (debug)
		fprintf(stderr, "debug:: abrir_conexion() bind()\t\t..........OK\n");

	/* Habilitamos el socket para recibir conexiones, con una cola de conexiones en espera que tendrá como máximo el tamaño especificado en BACKLOG*/

	if (backlog == 0)
		backlog = BACKLOG;

	if ((aux = listen(sockaux, backlog)) == -1)
	{
		fprintf(stderr, "Error en función listen. Código de error %s\n", strerror(aux));
		return -1;
	}
	if (debug)
		fprintf(stderr, "debug:: abrir_conexion() listen()\t..........OK\n");

	return sockaux;
}

/**
 * int aceptar_pedidos (int sockfd)
 * \brief Una vez recibido el pedido de conexión, esta función establece la conexión con el extremo remoto.
 * \details Cuando se recibe el pedido de conexión desde el extremo remoto al que se espera en segundo plano mediante la función listen (),
 * se recibe de dicha función el avviso para llamar a accept (). Ests función tiene  por objeto completar la conexión. Durante ese lapso se
 * ejecuta con el cliente reomoto un intercambio de paquetes de control que permiten establecer la conexión de manera confiable y segura.
 * El procecso estará en estado Sleeped hasta que termine esta actividad. Al terminar la función devolverá un  duplicado del socket para
 * ser utilizado en el intercambio de información con el extremo remoto, mientras si se lo desea, al mismmo tiempo se vuelva a escuchar
 * pedidos de conexión por el socket original.
 * \arg[in] sockeet originalmente creado por el que se recibió el pedido de conexión.
 * return Devuelve un entero con el file descriptor de socket duplicado para intercambio de datos
 * \author Alejandro Furfaro
 * \date 2018.09.10
 */

int aceptar_pedidos(int sockfd, int debug)
{
	int newfd;					   /* Por este socket duplicado del inicial se transaccionará*/
	struct sockaddr_in their_addr; /* Contendra la direccion IP y número de puerto del cliente */
	unsigned int sin_size = sizeof(struct sockaddr_in);

	/*Se espera por conexiones ,*/
	if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
	{
		fprintf(stderr, "Error en función accept. Código de error %s\n", strerror(newfd));
		return -1;
	}
	else
	{
		if (debug)
			fprintf(stderr, "debug:: aceptar_pedidos() conexión desde: %s\n", inet_ntoa(their_addr.sin_addr));

		return newfd;
	}
}
