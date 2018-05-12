#include <openssl/des.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
 
  char de_content[1024] = {0};
  char content_in[1024] = {0};
  char content_out[1024] = {0};
  int fd = open("./des_test.txt",O_CREAT|O_WRONLY);
  if(fd <= 0)
  {
    printf("fd fail\n");
    return 0; 
  }
  
  const char *key = "abc45678";
  const char *content = "1234567890";
  
  DES_cblock key_cblock;
   DES_string_to_key(key, &key_cblock);
     
   DES_key_schedule schedule;
   DES_set_key_checked(&key_cblock, &schedule);
  /*
  if(DES_enc_write(fd, content, strlen(content)+1,&schedule, &key_cblock) < 0)
  {
    printf("DES_enc_write fail\n");
    return 0; 
  }
  close(fd);
  fd = open("./des_test.txt",O_CREAT|O_RDWR);
  /*
  if(DES_enc_read(fd, de_content, 1024, &schedule, &key_cblock) < 0)
  {
    printf("DES_enc_write fail\n");
    return 0;     
  }
  
  printf("process sucessfull\n content=%s\n", de_content);
  if(read(fd,content_in,1024) < 0)
  {
    printf("read file failed\n");
    return 0; 
  }
  close(fd);
  printf("content_in=%s\n", content_in);*/
  printf("key_cblock= %s\n", key_cblock);
  //printf("content= %s\n", content);
  DES_ncbc_encrypt(content, content_in, strlen(content), &schedule, &key_cblock, DES_ENCRYPT  );
  
  DES_string_to_key(key, &key_cblock);
  DES_set_key_checked(&key_cblock, &schedule);
  printf("key_cblock= %s\n", key_cblock);
  DES_ncbc_encrypt(content_in, content_out, strlen(content_in), &schedule, &key_cblock, DES_DECRYPT );
  printf("content= %s\n", content);
  printf("content_in= %s\n", content_in);
  int i=0;
  for(; i<20; i++)
  {
    printf("content_in[%c]\n", content_in[i]); 
  }
  printf("content_out= %s\n", content_out);
  
   return 0;
   
   
}
