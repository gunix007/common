#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int main(void)
{
	int ret;
	int i = 0;
	const char *srcstr = "[{\"deviceId\":\"prot_200\",\"imei\":\"\",\"imsi\":\"460.02.3131362324\",\"tmsi\":\"\",\"isp\":0,\"uptime\":1524306405,\"homeArea\":\"\",\"netType\":\"4G\"}]";
	uLong srclen = strlen(srcstr) + 1;
	char *compr = NULL;
	char uncompr[200] = {0};
	uLong comprlen = 0;
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
	for (i = 0; i < comprlen; i++)
	{
		printf("%02x", *(compr+i));
	}
	printf("\n\n");

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

