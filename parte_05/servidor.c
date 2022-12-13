// gcc -Wall servidor.c sock-lib.c -o servidor
// envio de una struct

#include "sock-lib.h"
#include <stdint.h>
#include "producto.h"

int main()
{
    int sock, sockdup;

    product_t p1 = {
        .sku = "TSH-FF0000-L", // 13 bytes
        .color = 'R',          // 1 byte
        .quantity = 10,        // 4 bytes
    };                         // 18 bytes

    // printf("%ld\n", sizeof(p1)); // 20 bytes vs 18 bytes
    // return 0;

    sock = abrir_conexion(8000, 10, 1);
    if (sock == -1)
    {
        fprintf(stderr, "fallo la creacion de la conexion");
        exit(-1);
    }

    p1.quantity = htonl(p1.quantity);

    while (1)
    {
        // es bloqueante....
        sockdup = aceptar_pedidos(sock, 1);

        // write
        write(sockdup, &p1, sizeof(p1));

        close(sockdup);
    }
    close(sock);

    return 0;
}
