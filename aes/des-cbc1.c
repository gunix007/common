#include <stdio.h>
#include <string.h> 
#include <stdlib.h>  
#include <openssl/des.h> 

// DES-CBC加密方式  * 8位密鑰，加密內容8位補齊，補齊方式為填充0
int main(int argc, char *argv[])
{
	
	unsigned char buff[20]={"123412341234"};
	unsigned char encry_buff[20] ;
	int packet_len = strlen(buff);
	
	int i;
	int encry_len;
	memset(encry_buff,0,20);
		
	//密鑰
	DES_cblock KEY_DES = "pk$@gtjt";		
	//加密向量
	DES_cblock ivec = "thvn#&@@";	
	//解密向量
	DES_cblock ivec_de = "thvn#&@@";	
	DES_key_schedule schedule;			 	
	DES_set_key_unchecked((const_DES_cblock*)KEY_DES, &schedule);			 	
			 			 	
	encry_len = (strlen(buff)+7)/8*8;//長度		
		
	unsigned char *output=(char*)malloc(encry_len);	
	
	//加密
	memset(output,0,strlen(output));
	DES_ncbc_encrypt(buff, output, strlen(buff), &schedule, &ivec, DES_ENCRYPT);
    for (i = 0; i < encry_len; i++)
    {
        printf("%02x", output[i]);
    }
    printf("\n");

	//解密
 	memset(buff,0,20);
 	DES_ncbc_encrypt(output, buff, strlen(output), &schedule, &ivec_de, DES_DECRYPT);
    printf("decrypt:\n%s\n", buff);

 	free(output);
 	return 0;	 	
} 
