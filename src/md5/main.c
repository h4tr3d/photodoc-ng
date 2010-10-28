#include <stdio.h>
#include <string.h>

#include <openssl/md5.h>

#include "md5.h"

int main(int argc, char *argv[])
{
    uint8_t hash[16];
    int i;

    if(argc < 2)
    {
        printf("Use: %s <text>\n", argv[0]);
        return 1;
    }

    char *pass = argv[1];

    memset(hash, 0, sizeof(hash));
    md5hash(pass, strlen(pass), hash);
    printf("INTERNAL: ");

    for(i = 0; i < sizeof(hash);i++)
    {
        printf("%.2x", hash[i]);
    }
    printf("\n");

    memset(hash, 0, sizeof(hash));
    MD5(pass, strlen(pass), hash);
    printf(" OPENSSL: ");
    for(i = 0; i < sizeof(hash);i++)
    {
        printf("%.2x", hash[i]);
    }
    printf("\n");

    return 0;
}
