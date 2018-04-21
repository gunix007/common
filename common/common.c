#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 将十六进制的字符串转换成整数 */
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

/* 将字符数组表示成十六进制字符串格式 */
void char_array_to_hex_str(char *dst, const char *src)
{
	int i = 0;
	char strtmp[3] = {0};

	for (i = 0; i < strlen(src); i++)
    {
        sprintf(strtmp, "%02x", *(src+i));
        memcpy(&dst[i*2], strtmp, 2);
    }
}



