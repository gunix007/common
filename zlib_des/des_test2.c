#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/des_crypt.h>

#define BUFFSIZE	420

int main(void)
{
    int datalen;
    int result;
	char key[] = {"aBcDeFg1"};
	//char srcdata[BUFFSIZE] = {"mysrcdata1234test"};
	char srcdata[BUFFSIZE] = {"[{\"deviceId\":\"prot_200\",\"imei\":\"\",\"imsi\":\"460.11.0869053643\",\"tmsi\":\"\",\"isp\":2,\"uptime\":1524465247,\"homeArea\":\"\",\"netType\":\"4G\"},{\"deviceId\":\"prot_200\",\"imei\":\"\",\"imsi\":\"460.11.0417784373\",\"tmsi\":\"\",\"isp\":2,\"uptime\":1524465248,\"homeArea\":\"\",\"netType\":\"4G\"},{\"deviceId\":\"prot_200\",\"imei\":\"\",\"imsi\":\"460.11.0869053643\",\"tmsi\":\"\",\"isp\":2,\"uptime\":1524465251,\"homeArea\":\"\",\"netType\":\"4G\"}]"};
	char ivec[] = {"87654321"};
	char ivectemp[] = {"87654321"};

    des_setparity(key);

    datalen = strlen(srcdata);
    printf("datalen is: %d\n", datalen);
	/* Add to srcdata to ensure size is divisable by 8. */
	while (datalen % 8 && datalen < BUFFSIZE)
	{
		srcdata[datalen++] = '\0';
    }
	printf("orignal srcdata is:\n%s\n", srcdata);
	printf("datalen is: %d\n", datalen);
	printf("Encrypted:\n");
	result = cbc_crypt(key, srcdata, datalen, DES_ENCRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result) || strcmp(srcdata, "") == 0)
	{
		if (strcmp(srcdata, "") == 0)
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
		printf("cryption data: %s\n\n", srcdata);
    }

	//datalen = strlen(srcdata);
	printf("datalen is: %d\n", datalen);
	/* Add to srcdata to ensure size is divisable by 8. */
	while (datalen % 8 && datalen < BUFFSIZE)
	{
		srcdata[datalen++] = '\0';
	}
	printf("datalen is: %d\n", datalen);
	printf("Decrypted:\n");
	/* keeping the same initialization vector for decrypting, encrypt has altered ivectemp */
	strcpy(ivectemp, ivec);
	result = cbc_crypt(key, srcdata, datalen, DES_DECRYPT | DES_SW, ivectemp);
	if (DES_FAILED(result) || strcmp(srcdata, "") == 0)
	{
		if (strcmp(srcdata, "") == 0)
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
		printf("decrypt data:\n%s\n", srcdata);
	}

	return 0;
}

