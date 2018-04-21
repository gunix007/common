#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int main(void)
{
	int ret;
	const char *srcstr = "12345678901234567890123456789012345678901234567890";
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

