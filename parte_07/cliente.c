// gcc -Wall cliente.c sock-lib.c -o cliente
// recepcion de una struct serializada

/*

[set]
matches a non-empty sequence of character from set of characters.

If the first character of the set is ^, then all characters not in the set are matched.
If the set begins with ] or ^] then the ] character is also included into the set.
It is implementation-defined whether the character - in the non-initial position in the scanset
may be indicating a range, as in [0-9].
If width specifier is used, matches only up to width.
Always stores a null character in addition to the characters matched
(so the argument array must have room for at least width+1 characters)

*/


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
    sscanf(buffer, STRUCT_FORMAT_DES, p1.sku, &p1.color, &p1.quantity);

    fprintf(stderr, "read bytes = %d\n", r);
    fprintf(stderr, "%s\n", p1.sku);
    fprintf(stderr, "%c\n", p1.color);
    fprintf(stderr, "%d\n", p1.quantity);

    // cerrar el socket
    close(sock);

    return 0;
}