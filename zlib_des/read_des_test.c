#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static unsigned char srcdata[128*10000] = {0};

int main(void)
{
    int datalen = 0;
    int fd = 0;
    int readlen;
    int i;
    int result = 0;
	char key[] = "aBcDeFg1";
    unsigned char paddedValue = 0;
	char ivec[] = "87654321";
	char ivectemp[] = "87654321";

    fd = open("dazhan.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("open failed!\n");
        return errno;
    }

    readlen = read(fd, srcdata, sizeof(srcdata));
    if (readlen < 0)
    {
        printf("read imis data error\n");
        close(fd);
        return errno;
    }
    close(fd);
    printf("imse data:\n%s\n", srcdata);

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
		printf("decrypt data:\n%s\n", srcdata);
	}

	return 0;
}

