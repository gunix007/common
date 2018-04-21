#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include <zlib.h>

#define BUFFSIZE    420

int main(void)
{
	int ret;
	//const char *srcstr = "12345678901234567890123456789012345678901234567890";
	const char *srcstr = "[{\"deviceId\":\"prot_200\",\"imei\":\"\",\"imsi\":\"460.02.3131362324\",\"tmsi\":\"\",\"isp\":0,\"uptime\":1524306405,\"homeArea\":\"\",\"netType\":\"4G\"}]";
	uLong srclen = strlen(srcstr) + 1;
	char *compr = NULL;
	char uncompr[200] = {0};
	uLong comprlen = 0;

	int datalen;
	int result;
	char key[] = {"aBcDeFg1"};
	char ivec[] = {"87654321"};
	char ivectemp[] = {"87654321"};

	uLong uncomprlen = sizeof(uncompr);

	comprlen = compressBound(srclen);
	if (NULL == (compr = malloc(sizeof(char) * comprlen)))
	{
		printf("no enough memory\n");
		return 1;
	}
	printf("srclen: %ld, comprlen: %ld\n", srclen, comprlen);

	ret = compress(compr, &comprlen, srcstr, srclen);
	if (ret != Z_OK)
	{
		printf("compress error: %d", ret);
		return 1;
	}
	printf("orignal size: %ld, compressed size: %ld\n", srclen, comprlen);
	printf("compressed string: %s\n\n", compr);

	while (comprlen % 8 && comprlen < BUFFSIZE)
	{
		compr[comprlen++] = '\0';
	}
	result = cbc_crypt(key, compr, comprlen, DES_ENCRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result) || strcmp(compr, "") == 0)
	{
		if (strcmp(compr, "") == 0)
		{
			printf("*** Null Output ***\n");
		}
		else
		{
			printf("*** Encryption Error ***\n");
		}
	}
	else
	{
		printf("cryption data: %s\n\n", compr);
    }

	datalen = strlen(compr);
	printf("datalen is: %d\n", datalen);
	/* Add to srcdata to ensure size is divisable by 8. */
	while (datalen % 8 && datalen < BUFFSIZE)
	{
		compr[datalen++] = '\0';
	}
	printf("datalen is: %d\n", datalen);
	printf("Decrypted:\n");
	/* keeping the same initialization vector for decrypting, encrypt has altered ivectemp */
	strcpy(ivectemp, ivec);
	result = cbc_crypt(key, compr, datalen, DES_DECRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result) || strcmp(compr, "") == 0)
	{
		if (strcmp(compr, "") == 0)
		{
			printf("*** Null Output ***\n");
		}
		else
		{
			printf("*** Decryption Error ***\n");
		}
	}
	else
	{
		printf("decrypt data: %s\n", compr);
	}

	ret = uncompress(uncompr, &uncomprlen, compr, comprlen);
	if (ret != Z_OK)
	{
		printf("uncompress error: %d\n", ret);
		return 1;
	}
	printf("\norignal size: %ld, uncompressed size: %ld\n", srclen, uncomprlen);

	if (strcmp((char *)uncompr, srcstr))
	{
		printf("BAD uncompress!!!\n");
		return 1;
	}
	else
	{
		printf("uncompress() succeed\n%s\n", (char *)uncompr);
	}
}

