// gcc -Wall servidor.c sock-lib.c -o servidor

#include "sock-lib.h"

int main()
{
    int sock, sockdup;
    char msg[] = "hello world";

    sock = abrir_conexion(8000, 10, 1);
    // TODO... check

    // es bloqueante....
    sockdup = aceptar_pedidos(sock, 1);

    write(sockdup, msg, strlen(msg));
    // TODO... check

    close(sockdup);
    close(sock);

    return 0;
}
