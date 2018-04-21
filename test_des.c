#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <rpc/des_crypt.h>

void des_encrypt(const char *key, char *data, int len)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    des_setparity(pkey);
    do {
        data[len++] = '\x8';
    } while (len % 8 != 0);
    ecb_crypt(pkey, data, len, DES_ENCRYPT);
}

void des_decrypt(const char *key, char *data, int len)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    des_setparity(pkey);
    ecb_crypt(pkey, data, len, DES_DECRYPT);
}

int main(int argc, char *argv[])
{
	char data[4096] = "hello world";

	des_encrypt("ds", data, strlen(data));
	printf("data: %s\n", data);

	des_decrypt("ds", data, strlen(data));
	printf("\ndata2: %s\n", data);

    return 0;
}

