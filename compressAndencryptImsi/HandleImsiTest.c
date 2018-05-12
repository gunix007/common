#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "zlib.h"
#include "HandleImsi.h"

static char s_imsiCipherTextBuf[1024*12] = {0};
/* s_rawImsiData 要足够大以容纳下最大的原始 Imsi 数据 */
static char s_rawImsiData[1024*20] = {0};
/* s_imsiPlainTextBuf 要足够大以容纳下最大的还原后的 Imsi 数据 */
static char s_imsiPlainTextBuf[1024*20] = {0};

/* key 取自属性里面的 key */
char key[] = "93ccb53bbb49443386fdby6gtt98v186";

int main(int argc, char *argv[])
{
	int resultLen = 0;
	int fd = 0;
	int readlen = 0;

	fd = open("orig.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("open failed!\n");
		return errno;
	}

	readlen = read(fd, s_rawImsiData, sizeof(s_rawImsiData));
	if (readlen < 0)
	{
		printf("read imis data error\n");
		close(fd);
		return errno;
	}
	close(fd);
	printf("imse data: %s\n", s_rawImsiData);

    resultLen = CompressAndEncryptImsiData(s_imsiCipherTextBuf,
            sizeof(s_imsiCipherTextBuf), s_rawImsiData, key);
	if (resultLen < 0)
	{
		printf("CompressAndEncryptImsiData failed!\n");
		return -1;
	}
    printf("resultLen: %d, strlen(s_imsiCipherTextBuf): %d\n", resultLen, strlen(s_imsiCipherTextBuf));

	resultLen = DecryptAndUncompressImsiData(s_imsiPlainTextBuf,
            sizeof(s_imsiPlainTextBuf), s_imsiCipherTextBuf, key);
	if (resultLen < 0)
	{
		printf("DecryptAndUncompressImsiData failed!\n");
		return -2;
	}
	printf("\nuncompress string: %s\n", s_imsiPlainTextBuf);
    printf("resultLen: %d, strlen: %d\n", resultLen, strlen(s_imsiPlainTextBuf));

	return 0;
}


