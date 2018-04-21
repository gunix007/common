#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>

int main()  
{     
    clock_t a,b;  
    a = clock();  
    DES_Encrypt("1.txt","key.txt","2.txt");  
    b = clock();  
    printf("加密消耗%d毫秒\n",b-a);  
      
    system("pause");  
    a = clock();  
    DES_Decrypt("2.txt","key.txt","3.txt");  
    b = clock();  
    printf("解密消耗%d毫秒\n",b-a);  
    getchar();  
    return 0;  
}   
