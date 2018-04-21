#include <stdio.h>
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

void rc4_init(unsigned char *s, unsigned char *key, unsigned long len)
{
	int i = 0, j = 0;
	char k[256] = {0};
	unsigned char tmp = 0;

	for (i = 0; i < 256; i++)
	{
		s[i] = i;
		k[i] = key[i%len];
	}

	for (i = 0; i < 256; i++)
	{
		j = (j + s[i] + k[i]) % 256;
		tmp = s[i];
		s[i] = s[j]; // exchange s[i] and s[j]
		s[j] = tmp;
    }
}

/* encrypt and decrypt function */
void rc4_crypt(unsigned char *s, unsigned char *Data, unsigned long len)
{
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;

	for (k = 0; k < len; k++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j]; // exchange s[i] and s[j]
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		Data[k] ^= s[t];
    }
}

int main(int argc, char *argv[])
{
	unsigned char s[256] = {0}, s2[256] = {0}, s3[256] = {0}; // S-box
	char key[256] = "93ddb53bbb49443386fdb3ggtt98v185";
	char pData[512] = "helloworld";
	char pData2[2048] = "ce1330daaf117fa6ecafed2884ee535eaa21d65cdcc2a9dd8e371c03fc7bd89ef19f4da739e684ef59ee1b1a3c6c0d3890aded9c17ea4b1536a2ac35d4e71070b0e653f0af0eecc43f1a7f4b70424b1658bae556b745b170bb1cf53c930c9719b94c3ab1ba083a2eb785d8c228cf5cc6de646269c6b79d84860aab6c541a4b5d723130293af39c4b42bd547743637f41e1f8b40b8ed239ef70ee2255136d8b340dff181d905c6c3fd4a666f0f88b45cf77e1a26c7493f0bce5fe022468c1b737d4f8a8230d0f812ec48fe0bfff30966b92aae565244c8b9399bb0da796ab021e8b20792777b1f9e14598d9f22b99e0686959c6f3909a22e01ed746953a2315bf811b54084b48f28c05ea2a8cce3c838019694e40f2d01313a0c7c3b1a9506a50d4db8fc9dbf4a74d76213a45edab0d86bcfaa3a74a1bc587047b549e9591778e194976b6838259669160b30b60a55c8c75cf04b4ce3afbf10c6d0aed004111ba4229f12d1e0be01f18888864d8ef908f739c1b239bdc1f1218355c0701d150e1d8474e5eb98030a2074b2e90117a6045c1658cab99d334377fd493961817784eb32a7e83116f88d561e3efc4e6388ebf1ec1020011b65b3fa3c1ea0af224300184bc3a8f374efdc37dcc20a60df27fcc839e22376dd460c674ff92c265f4ad94466eb4c0e9653ab3d1ec6f2177b092d0ec0e72dca09efbb4b4ffc3e99dd6475fe383b6c1a263c10e6d87383393d2050c44754b23837f8bc3fa7aa2d2337c4c361b3de8372012b570de5c5c2b39bd0c7843f1d4743251f69e23f0e44c510aa982be04d61824334042729d9628797a6fbb9314fa6ede09e015edf331c4b2315a3e1badea584b1798eee0a5c49d78fb4fc5feede743d1c56ad19075b5ee4813d729a78331414467ff4bc88677395fb5e6bce783bcbfa39dee2f397bd123306717dec377ef38cdeac0eec87d1e8a484108f16d6001c15c0a1993ec4940a5315a9a584df134e21e7bc7a89ea8a57c894197884bdc5630800c8432469fb5c16de82798e10e51d72d5e66200eb4a0ef906f1cbc131106073d5946fbc8fc11cabb769e85c5d740fc51a866d95f907dbb8dcfeb9047464341d228eb9dbd365ab2864d4241026db7962153ebac6548484715795bedaa3f3bf7ff1dd7b1030a8fe456d9a5051f43881e22b6a07e649963e9da74aa85730526e717218a4a093a704c30fbef6f16ab351bdb299e2234e9d92f61b5791aa94eed562a36be42facc0c779a2726a531f75d642028621cbc43c8d60798a4bcb7e46b3cd5075dbf7adcbc4cfc490798be65d3a15754a41a413ac4c2f9d40fc7f2e365b3a23eab392ff87613f5c587e92e1a0b7cb5330ca9ccb13e5785678cc21503";
	char imsidata[1024] = {0};
	char strtmp[3] = {0};
	unsigned long len = strlen(pData);
	unsigned long len2 = strlen(pData2);
	int i;

	printf("pData = %s\n", pData);
	printf("key = %s, length = %d\n\n", key, strlen(key));
	rc4_init(s, (unsigned char *)key, strlen(key)); // 初始化
	printf("完成对 S[i] 的初始化，如下：\n\n");
	for (i = 0; i < 256; i++)
	{
		printf("%d", s[i]);
		if (i && (i + 1) % 16 == 0)
		{
			putchar('\n');
		}
    }
	printf("\n\n");
	for (i = 0; i < 256; i++) // 用 s2[i] 暂时保留经过初始化的 s[i]，很重要的！！！
	{
		s2[i] = s[i];
	}
	for (i = 0; i < 256; i++) // 用 s3[i] 暂时保留经过初始化的 s[i]，很重要的！！！
	{
		s3[i] = s[i];
	}
	printf("已经初始化，现在加密:\n\n");
	rc4_crypt(s, (unsigned char *)pData, len); // 加密
	printf("pData = %s\n\n", pData);
	printf("\n已经加密，现在解密:\n\n");
	rc4_crypt(s2, (unsigned char *)pData, len); // 解密
	printf("pData = %s\n\n", pData);
	for (i = 0; i < len2/2; i++)
	{
		memcpy(strtmp, &pData2[i*2], 2);
		imsidata[i] = htoi(strtmp);
	}
	rc4_crypt(s3, (unsigned char *)imsidata, len2/2); // 解密
	printf("pData = %s\n\n", imsidata);

	return 0;
}

