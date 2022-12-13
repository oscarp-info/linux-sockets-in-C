// gcc -Wall cliente.c sock-lib.c -o cliente
// envio y recepcion de un "int" (int32_t)

#include "sock-lib.h"
#include <stdint.h>

#define SYMBOL 0xAABBCCDD

int main()
{
    int sock;
    int32_t entero = SYMBOL;

    // conectarse
    sock = conectar("localhost", 8000, 1);

    // write
    entero = htonl(entero);
    write(sock, &entero, sizeof(int32_t));

    // read
    read(sock, &entero, sizeof(int32_t));
    entero = ntohl(entero);
    fprintf(stderr, "debug rx=%x\n", entero);

    // cerrar el socket
    close(sock);

    return 0;
}