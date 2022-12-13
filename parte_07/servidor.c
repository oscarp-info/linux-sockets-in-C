// gcc -Wall servidor.c sock-lib.c -o servidor -lpthread
// envio de una struct serializada
// multithread
// ps -T -p <pid>

#include "sock-lib.h"
#include <stdint.h>
#include "producto.h"
#include <pthread.h>

void *process(void *ptr);

typedef struct
{
    int sock;
    // ....
} connection_t;

int main()
{
    int sock;
    connection_t *connection;
    pthread_t tid;

    sock = abrir_conexion(8000, 10, 1);
    if (sock == -1)
    {
        fprintf(stderr, "fallo la creacion de la conexion");
        exit(-1);
    }

    while (1)
    {
        // es bloqueante....
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = aceptar_pedidos(sock, 1);

        if (connection->sock > 0)
        {
            pthread_create(&tid, 0, process, (void *)connection);
            pthread_detach(tid);
        }
        else
        {
            free(connection);
        }
    }
    close(sock);

    return 0;
}

void *process(void *ptr)
{
    product_t p1 = {
        .sku = "TSH-FF0000-L", // 13 bytes
        .color = 'R',          // 1 byte
        .quantity = 10,        // 4 bytes
    };                         // 18 bytes

    char buffer[100];

    memset(buffer, 0, 100);
    sprintf(buffer, "{%s,%c,%d}", p1.sku, p1.color, p1.quantity);
    // fprintf(stderr, "%s\n", buffer);

    connection_t *conn;
    conn = (connection_t *)ptr;

    // write
    write(conn->sock, buffer, strlen(buffer));

    close(conn->sock);

    // sleep(60);

    free(conn);

    return NULL;
}