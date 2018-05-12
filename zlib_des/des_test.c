#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>

#define BUFFSIZE	420

int main(void)
{
    int i;
    int datalen = 0;
    int result = 0;
	char key[] = "aBcDeFg1";
    char paddedValue = 0;
	unsigned char srcdata[BUFFSIZE] = "mysrcdata1234test";
	char ivec[] = "87654321";
	char ivectemp[] = "87654321";

    des_setparity(key);
    datalen = strlen(srcdata);
    printf("datalen is: %d\n", datalen);
	/* Add to srcdata to ensure size is divisable by 8. */
    paddedValue = 8 - datalen % 8;
	while (datalen % 8)
	{
		// srcdata[datalen++] = 0; // zeropadding
		srcdata[datalen++] = paddedValue; // pkcs5padding
    }
	printf("orignal srcdata is: %s\n", srcdata);
	printf("datalen is: %d\n", datalen);
	printf("Encrypted:\n");
	result = cbc_crypt(key, srcdata, datalen, DES_ENCRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result))
	{
        printf("*** Encryption Error ***\n");
	}

	datalen = strlen(srcdata);
	printf("datalen is: %d\n", datalen);
	/* Add to srcdata to ensure size is divisable by 8. */
    paddedValue = 8 - datalen % 8;
	while (datalen % 8)
	{
		// srcdata[datalen++] = '\0';
		srcdata[datalen++] = paddedValue;
	}
	printf("datalen is: %d\n", datalen);
	printf("Decrypted:\n");
	/* keeping the same initialization vector for decrypting, encrypt has altered ivectemp */
	strcpy(ivectemp, ivec);
	result = cbc_crypt(key, srcdata, datalen, DES_DECRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result))
	{
        printf("*** Decryption Error ***\n");
	}
	else
	{
		printf("decrypt data: %s\n", srcdata);
	}

	return 0;
}

