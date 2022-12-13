// gcc -Wall cliente.c sock-lib.c -o cliente
// recepcion de una struct serializada

#include "sock-lib.h"
#include <stdint.h>
#include "producto.h"

int main()
{
    int sock;
    product_t p1;
    int r;
    char buffer[100];

    // conectarse
    sock = conectar("localhost", 8000, 1);

    memset(buffer, 0, 100);
    r = read(sock, buffer, sizeof(buffer));

    fprintf(stderr, "%s\n", buffer);
    // "{TSH-FF0000-L,R,10}"
    sscanf(buffer, "{%[^,],%c,%d}", p1.sku, &p1.color, &p1.quantity);

    fprintf(stderr, "read bytes = %d\n", r);
    fprintf(stderr, "%s\n", p1.sku);
    fprintf(stderr, "%c\n", p1.color);
    fprintf(stderr, "%d\n", p1.quantity);

    // cerrar el socket
    close(sock);

    return 0;
}