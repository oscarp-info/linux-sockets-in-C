// gcc -Wall servidor.c sock-lib.c -o servidor

#include "sock-lib.h"

int main()
{
    int sock, sockdup;
    char msg[] = "hello world";
    int r;

    sock = abrir_conexion(8000, 10, 1);
    if (sock == -1)
    {
        fprintf(stderr, "fallo la creacion de la conexion");
        exit(-1);
    }

    while (1)
    {
        // es bloqueante....
        sockdup = aceptar_pedidos(sock, 1);

        r = write(sockdup, msg, strlen(msg));
        if (r == -1)
        {
            fprintf(stderr, "fallo la escritura");
            exit(-2);
        }
        // comentar para verificar los sockets desde la linea de comandos
        close(sockdup);
    }
    close(sock);

    return 0;
}
