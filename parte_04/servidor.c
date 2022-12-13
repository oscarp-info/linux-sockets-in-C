// gcc -Wall servidor.c sock-lib.c -o servidor
// recepcion y envio de un "int" (primero lo desplazo 1 byte para simular una tarea)

#include "sock-lib.h"
#include <stdint.h>

int main()
{
    int sock, sockdup;
    int32_t entero;

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

        read(sockdup, &entero, sizeof(int32_t));
        entero = ntohl(entero);
        fprintf(stderr, "debug rx=%x\n", entero);

        // simular una accion
        entero = entero << 8;
        entero = htonl(entero);
        write(sockdup, &entero, sizeof(int32_t));

        close(sockdup);
    }
    close(sock);

    return 0;
}
