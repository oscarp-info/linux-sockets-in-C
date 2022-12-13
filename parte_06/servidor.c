// gcc -Wall servidor.c sock-lib.c -o servidor
// envio de una struct serializada

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

    char buffer[100];

    sock = abrir_conexion(8000, 10, 1);
    if (sock == -1)
    {
        fprintf(stderr, "fallo la creacion de la conexion");
        exit(-1);
    }

    memset(buffer, 0, 100);
    sprintf(buffer, "{%s,%c,%d}", p1.sku, p1.color, p1.quantity);
    fprintf(stderr, "%s\n", buffer);

    while (1)
    {
        // es bloqueante....
        sockdup = aceptar_pedidos(sock, 1);

        // write
        write(sockdup, buffer, strlen(buffer));

        close(sockdup);
    }
    close(sock);

    return 0;
}
