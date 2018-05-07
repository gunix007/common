#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include <zlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define srv_secret "135d7b54fae14a54a8c747ed3a01d9f7"

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

char *CompressImsiData(char *imsiData, uLong *compressLen)
{
    int ret = 0;
    unsigned char *compressBuf = NULL;
    int imsiDataLen = strlen(imsiData) + 1;
    uLong tmpCompressLen = 0;
    uLong tmpCompressLenBound = 0;

    tmpCompressLen = compressBound(imsiDataLen);
    tmpCompressLenBound = tmpCompressLen;
    if (NULL == (compressBuf = malloc(sizeof(char) * tmpCompressLen + 1)))
    {
        printf("no enough memory\n");
        return NULL;
    }

    ret = compress(compressBuf, &tmpCompressLen, imsiData, imsiDataLen);
    if (ret != Z_OK)
    {
        printf("compress error: %d", ret);
        free(compressBuf);
        return NULL;
    }
    *compressLen = tmpCompressLen;

    printf("compressImsidata imsiData: imsiDataLen: %d, tmpCompressLenBound: %ld, tmpCompressLen: %ld\n", 
        imsiDataLen, tmpCompressLenBound, tmpCompressLen);

    return compressBuf;
}

int encryptImsidata(char *src, int len)
{
    int result = 0;
    int remainder;
    int byteValue;
    int fillNum;
    int i;
    unsigned int srclen = 0;
    char key[64] = {0};
    char ivec[] = {"87654321"};

    strncpy(key, srv_secret, strlen(srv_secret));
    des_setparity(key);
    srclen = len;

    remainder = srclen % 8;
	if (remainder)
	{
		byteValue = 8 - remainder;
		fillNum = 8 - remainder;
		for (i = 0; i < fillNum; i++)
		{
			src[srclen + i] = byteValue;
		}
		srclen += fillNum;
	}
    printf("encryptImsidata beforelen: %d, byteValue: %d, fillNum: %d, srclen: %d, \n%s\n",
			len, byteValue, fillNum, srclen, src);

    result = cbc_crypt(key, src, srclen, DES_ENCRYPT | DES_SW, ivec);
    if (DES_FAILED(result))
    {
        printf("*** Encryption Error ***\n");
        return -1;
    }

    return srclen;
}

#define MAXLEN			4096*20

int decryptImsidata(char *src, int len)
{
	int result = 0;
	int remainder;
	int byteValue;
	int fillNum;
	int i;
	unsigned int srclen = 0;
	char key[64] = {0};
	char ivec[] = {"87654321"};

	strncpy(key, srv_secret, strlen(srv_secret));
	des_setparity(key);
	srclen = len;

	remainder = srclen % 8;
	if (remainder)
	{
		byteValue = 8 - remainder;
		fillNum = 8 - remainder;
		for (i = 0; i < fillNum; i++)
		{
			src[srclen + i] = byteValue;
		}
		srclen += fillNum;
	}
	printf("decryptImsidata beforelen: %d, byteValue: %d, fillNum: %d, srclen: %d \n\n",
			len, byteValue, fillNum, srclen);
	printf("Decrypted:\n");
	result = cbc_crypt(key, src, srclen, DES_DECRYPT | DES_SW, ivec);
	if (DES_FAILED(result))
	{
		printf("*** Encryption Error ***\n");
	}

    return 0;
}

char imsibuf[4096*10] = {0};
char sendimsibuf[4096*10] = {0};
char recvimsibuf[4096*10] = {0};
char uncompressBuf[4096*10] = {0};

int main(void)
{
    int ret = 0;
	int encryptLen = 0;
	int i = 0;
	int fd = 0;
	int readlen = 0;
	char strtmp[3] = {0};
    unsigned char *compressBuf = NULL;
    uLong compressLen = 0;
    uLong uncompresslen = sizeof(uncompressBuf);

	/********** open and read imsi data from file, start **********/
	fd = open("orig.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("open failed!\n");
		return errno;
	}

	readlen = read(fd, imsibuf, sizeof(imsibuf));
	if (readlen < 0)
	{
		printf("read imis data error\n");
		close(fd);
		return errno;
	}
	close(fd);
	printf("imse data: %s\n", imsibuf);
	/********** open and read imsi data from file, end **********/

	/********** compress imsi data, start **********/
	compressBuf = CompressImsiData(imsibuf, &compressLen);
	if (NULL == compressBuf)
	{
		printf("compress imsi data error\n");
		return -1;
	}

	printf("\ncompress data start\n");
	for (i = 0; i < compressLen; i++)
	{
		printf("%02x", compressBuf[i]);
	}
	printf("\ncompress data end\n");
	/********** compress imsi data, end **********/

	/********** encrypt the compressed imsi data, start **********/
	encryptLen = encryptImsidata(compressBuf, compressLen);
	if (encryptLen <= 0)
	{
		free(compressBuf);
		compressBuf = NULL;
		return -2;
	}
	for (i = 0; i < encryptLen; i++)
	{
		sprintf(strtmp, "%02x", compressBuf[i]);
		memcpy(&sendimsibuf[i*2], strtmp, 2);
	}
	free(compressBuf);
	compressBuf = NULL;
	/********** encrypt the compressed imsi data, end **********/

	/********** decrypt the compressed imsi data, start **********/
	printf("\nstrlen(sendimsibuf): %ld, sendimsibuf: %s\n", strlen(sendimsibuf), sendimsibuf);
	for (i = 0; i < strlen(sendimsibuf)/2; i++)
	{
		memcpy(strtmp, &sendimsibuf[i*2], 2);
		recvimsibuf[i] = htoi(strtmp);
	}
	printf("\ni: %d, strlen(sendimsibuf)/2: %ld\n", i, strlen(sendimsibuf)/2);
	decryptImsidata(recvimsibuf, strlen(sendimsibuf)/2);
	//printf("******* imis after decrypt: %s\n\n strlen(recvimsibuf): %d\n\n", recvimsibuf, strlen(recvimsibuf));
	/********** decrypt the compressed imsi data, end **********/

    ret = uncompress(uncompressBuf, &uncompresslen, recvimsibuf, strlen(sendimsibuf)/2);
	if (ret != Z_OK)
	{
		printf("uncompress error: %d\n", ret);
		return -3;
	}
	printf("\norig string: %s, uncompressed size: %ld\n", uncompressBuf, uncompresslen);

    return 0;
}

