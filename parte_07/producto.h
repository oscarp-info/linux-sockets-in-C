#ifndef _PRODUCTO_H
#define _PRODUCTO_H

#define SKU_LEN 13

typedef struct
{
    char sku[SKU_LEN]; // Stock Keeping Unit
    char color;
    int quantity;
} product_t;

// serialziar/deserializar
const char *STRUCT_FORMAT_SER = "{%s,%c,%d}";
const char *STRUCT_FORMAT_DES = "{%[^,],%c,%d}";

#endif