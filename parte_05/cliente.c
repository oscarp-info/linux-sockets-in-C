// gcc -Wall cliente.c sock-lib.c -o cliente
// recepcion de una struct

#include "sock-lib.h"
#include <stdint.h>
#include "producto.h"

int main()
{
    int sock;
    product_t p1;
    int r;

    // conectarse
    sock = conectar("localhost", 8000, 1);

    // read
    r = read(sock, &p1, sizeof(product_t));
    p1.quantity = ntohl(p1.quantity);

    fprintf(stderr, "bytes read=%d\n", r);
    fprintf(stderr, "p1.sku=%s\n", p1.sku);
    fprintf(stderr, "p1.color=%c\n", p1.color);
    fprintf(stderr, "p1.quantity=%d\n", p1.quantity);

    // cerrar el socket
    close(sock);

    return 0;
}