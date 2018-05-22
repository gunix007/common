#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/des_crypt.h>

unsigned char imsiData[1024*500] = {0};
unsigned char imsiDatahex[1024*1000] = {0};
unsigned char imsiData2[1024*500] = {0};

char key[] = "135d7b54fae14a54a8c747ed3a01d9f7";

int htoi(char s[])
{
	int i;
	int n = 0;
	if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
	{
		i = 2;
	}
	else
	{
		i = 0;
	}

	for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z'); ++i)
	{
		if (tolower(s[i]) > '9')
		{
			n = 16 * n + (10 + tolower(s[i]) - 'a');
		}
		else
		{
			n = 16 * n + (tolower(s[i]) - '0');
		}
	}

	return n;
}

int encryptImsidata(unsigned char *src, int len)
{
    int i = 0;
    int result;
    int eLen;
    int encryptLen;
    char ivec[] = "87654321";
    char paddedValue;

    des_setparity(key);

    paddedValue = 8 - len % 8;
    while (len % 8)
    {
        //src[len++] = 0;
        src[len++] = paddedValue;
    }
    printf("len is: %d\n", len);
    encryptLen = len;

    while (len > 0)
    {
        eLen = (len > DES_MAXDATA) ? DES_MAXDATA : len;
        result = cbc_crypt(key, src + DES_MAXDATA*i, eLen, DES_ENCRYPT | DES_SW, ivec);
        if (DES_FAILED(result))
        {
            printf("*** Encryption Error ***\n");
            return -1;
        }
        len -= DES_MAXDATA;
        i++;
    }

    return encryptLen;
}

int decryptImsidata(char *src, int len)
{
    int i = 0;
    int result = 0;
    int eLen = 0;
    char ivec[] = "87654321";
    char paddedValue;

    des_setparity(key);
    printf("len is: %d\n", len);
    paddedValue = 8 - len % 8;
    while (len % 8)
    {
        //src[len++] = 0;
        src[len++] = paddedValue;
    }
    printf("len is: %d\n", len);
    printf("Decrypted:\n");
    while (len > 0)
    {
        eLen = (len > DES_MAXDATA) ? DES_MAXDATA : len;
        result = cbc_crypt(key, src + DES_MAXDATA*i, eLen, DES_DECRYPT | DES_SW, ivec);
        if (DES_FAILED(result))
        {
            printf("*** Decryption Error ***\n");
            return -1;
        }
        len -= DES_MAXDATA;
        i++;
    }

    return 0;
}


int main(int argc, char *argv[])
{
    int i;
    int ret;
    int fd;
    int readlen;
    unsigned char strtmp[3] = {0};

    if (2 != argc)
    {
        printf("usage:\n\t%s datafile\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("open failed!\n");
        return errno;
    }
    readlen = read(fd, imsiData, sizeof(imsiData));
    //readlen = read(fd, imsiData, 8100);
    if (readlen < 0)
    {
        printf("read imis data error\n");
        close(fd);
        return errno;
    }
    //imsiData[readlen] = 0;
    close(fd);
    printf("imse data:\n%s\n", imsiData);
    printf("readlen: %d, strlen(imsiData): %d\n", readlen, (int)strlen(imsiData));

    ret = encryptImsidata(imsiData, readlen);
    if (ret < 0)
    {
        printf("encryptImsidata() failed\n");
        return -2;
    }

    for (i = 0; i < ret; i++)
    {
        sprintf(strtmp, "%02x", imsiData[i]);
        memcpy(&imsiDatahex[i*2], strtmp, 2);
    }
    imsiDatahex[i*2] = 0;
    printf("imsiDatahex:\n%s\n", imsiDatahex);

    /****************************************/

    printf("************************************\n");
    for (i = 0; i < strlen(imsiDatahex)/2; i++)
    {
        memcpy(strtmp, &imsiDatahex[i*2], 2);
        imsiData2[i] = htoi(strtmp);
    }
    decryptImsidata(imsiData2, i);
    printf("decrypt data:\n\n%s\n", imsiData2);

    return 0;
}

