#include "base.h"

//字节转换成二进制  
int ByteToBit(char ch, char bit[8]){  
    int cnt;  
    for(cnt = 0;cnt < 8; cnt++){  
        *(bit+cnt) = (ch>>cnt)&1;  
    }  
    return 0;  
}  
  
//二进制转换成字节  
int BitToByte(char bit[8],char *ch){  
    int cnt;  
    for(cnt = 0;cnt < 8; cnt++){  
        *ch |= *(bit + cnt)<<cnt;  
    }  
    return 0;  
}  
  
//将长度为8的字符串转为二进制位串  
int Char8ToBit64(char ch[8],char bit[64]){  
    int cnt;  
    for(cnt = 0; cnt < 8; cnt++){          
        ByteToBit(*(ch+cnt),bit+(cnt<<3));  
    }  
    return 0;  
}  
  
//将二进制位串转为长度为8的字符串  
int Bit64ToChar8(char bit[64],char ch[8]){  
    int cnt;  
    memset(ch,0,8);  
    for(cnt = 0; cnt < 8; cnt++){  
        BitToByte(bit+(cnt<<3),ch+cnt);  
    }  
    return 0;  
}  
  
//生成子密钥  
int DES_MakeSubKeys(char key[64],char subKeys[16][48]){  
    char temp[56];  
    int cnt;  
    DES_PC1_Transform(key,temp);//PC1置换  
    for(cnt = 0; cnt < 16; cnt++){//16轮跌代，产生16个子密钥  
        DES_ROL(temp,MOVE_TIMES[cnt]);//循环左移  
        DES_PC2_Transform(temp,subKeys[cnt]);//PC2置换，产生子密钥  
    }  
    return 0;  
}  
  
//密钥置换1  
int DES_PC1_Transform(char key[64], char tempbts[56]){  
    int cnt;      
    for(cnt = 0; cnt < 56; cnt++){  
        tempbts[cnt] = key[PC_1[cnt]];  
    }  
    return 0;  
}  
  
//密钥置换2  
int DES_PC2_Transform(char key[56], char tempbts[48]){  
    int cnt;  
    for(cnt = 0; cnt < 48; cnt++){  
        tempbts[cnt] = key[PC_2[cnt]];  
    }  
    return 0;  
}  
  
//循环左移  
int DES_ROL(char data[56], int time){     
    char temp[56];  
  
    //保存将要循环移动到右边的位  
    memcpy(temp,data,time);  
    memcpy(temp+time,data+28,time);  
      
    //前28位移动  
    memcpy(data,data+time,28-time);  
    memcpy(data+28-time,temp,time);  
  
    //后28位移动  
    memcpy(data+28,data+28+time,28-time);  
    memcpy(data+56-time,temp+time,time);      
  
    return 0;  
}  
  
//IP置换  
int DES_IP_Transform(char data[64]){  
    int cnt;  
    char temp[64];  
    for(cnt = 0; cnt < 64; cnt++){  
        temp[cnt] = data[IP_Table[cnt]];  
    }  
    memcpy(data,temp,64);  
    return 0;  
}  
  
//IP逆置换  
int DES_IP_1_Transform(char data[64]){  
    int cnt;  
    char temp[64];  
    for(cnt = 0; cnt < 64; cnt++){  
        temp[cnt] = data[IP_1_Table[cnt]];  
    }  
    memcpy(data,temp,64);  
    return 0;  
}  
  
//扩展置换  
int DES_E_Transform(char data[48]){  
    int cnt;  
    char temp[48];  
    for(cnt = 0; cnt < 48; cnt++){  
        temp[cnt] = data[E_Table[cnt]];  
    }     
    memcpy(data,temp,48);  
    return 0;  
}  
  
//P置换  
int DES_P_Transform(char data[32]){  
    int cnt;  
    char temp[32];  
    for(cnt = 0; cnt < 32; cnt++){  
        temp[cnt] = data[P_Table[cnt]];  
    }     
    memcpy(data,temp,32);  
    return 0;  
}  
  
//异或  
int DES_XOR(char R[48], char L[48] ,int count){  
    int cnt;  
    for(cnt = 0; cnt < count; cnt++){  
        R[cnt] ^= L[cnt];  
    }  
    return 0;  
}  
  
//S盒置换  
int DES_SBOX(char data[48]){  
    int cnt;  
    int line,row,output;  
    int cur1,cur2;  
    for(cnt = 0; cnt < 8; cnt++){  
        cur1 = cnt*6;  
        cur2 = cnt<<2;  
          
        //计算在S盒中的行与列  
        line = (data[cur1]<<1) + data[cur1+5];  
        row = (data[cur1+1]<<3) + (data[cur1+2]<<2)  
            + (data[cur1+3]<<1) + data[cur1+4];  
        output = S[cnt][line][row];  
  
        //化为2进制  
        data[cur2] = (output&0X08)>>3;  
        data[cur2+1] = (output&0X04)>>2;  
        data[cur2+2] = (output&0X02)>>1;  
        data[cur2+3] = output&0x01;  
    }     
    return 0;  
}  
  
//交换  
int DES_Swap(char left[32], char right[32]){  
    char temp[32];  
    memcpy(temp,left,32);     
    memcpy(left,right,32);    
    memcpy(right,temp,32);  
    return 0;  
}  
  
//加密单个分组  
int DES_EncryptBlock(char plainBlock[8], char subKeys[16][48], char cipherBlock[8]){  
    char plainBits[64];  
    char copyRight[48];  
    int cnt;  
  
    Char8ToBit64(plainBlock,plainBits);       
    //初始置换（IP置换）  
    DES_IP_Transform(plainBits);  
  
    //16轮迭代  
    for(cnt = 0; cnt < 16; cnt++){         
        memcpy(copyRight,plainBits+32,32);  
        //将右半部分进行扩展置换，从32位扩展到48位  
        DES_E_Transform(copyRight);  
        //将右半部分与子密钥进行异或操作  
        DES_XOR(copyRight,subKeys[cnt],48);   
        //异或结果进入S盒，输出32位结果  
        DES_SBOX(copyRight);  
        //P置换  
        DES_P_Transform(copyRight);  
        //将明文左半部分与右半部分进行异或  
        DES_XOR(plainBits,copyRight,32);  
        if(cnt != 15){  
            //最终完成左右部的交换  
            DES_Swap(plainBits,plainBits+32);  
        }  
    }  
    //逆初始置换（IP^1置换）  
    DES_IP_1_Transform(plainBits);  
    Bit64ToChar8(plainBits,cipherBlock);  
    return 0;  
}  
  
//解密单个分组  
int DES_DecryptBlock(char cipherBlock[8], char subKeys[16][48],char plainBlock[8]){  
    char cipherBits[64];  
    char copyRight[48];  
    int cnt;  
  
    Char8ToBit64(cipherBlock,cipherBits);         
    //初始置换（IP置换）  
    DES_IP_Transform(cipherBits);  
      
    //16轮迭代  
    for(cnt = 15; cnt >= 0; cnt--){        
        memcpy(copyRight,cipherBits+32,32);  
        //将右半部分进行扩展置换，从32位扩展到48位  
        DES_E_Transform(copyRight);  
        //将右半部分与子密钥进行异或操作  
        DES_XOR(copyRight,subKeys[cnt],48);       
        //异或结果进入S盒，输出32位结果  
        DES_SBOX(copyRight);  
        //P置换  
        DES_P_Transform(copyRight);       
        //将明文左半部分与右半部分进行异或  
        DES_XOR(cipherBits,copyRight,32);  
        if(cnt != 0){  
            //最终完成左右部的交换  
            DES_Swap(cipherBits,cipherBits+32);  
        }  
    }  
    //逆初始置换（IP^1置换）  
    DES_IP_1_Transform(cipherBits);  
    Bit64ToChar8(cipherBits,plainBlock);  
    return 0;  
}  
  
//加密文件  
int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile){  
    FILE *plain,*cipher;  
    int count;  
    char plainBlock[8],cipherBlock[8],keyBlock[8];  
    char bKey[64];  
    char subKeys[16][48];  
    if((plain = fopen(plainFile,"rb")) == NULL){  
        return PLAIN_FILE_OPEN_ERROR;  
    }     
    if((cipher = fopen(cipherFile,"wb")) == NULL){  
        return CIPHER_FILE_OPEN_ERROR;  
    }  
    //设置密钥  
    memcpy(keyBlock,keyStr,8);  
    //将密钥转换为二进制流  
    Char8ToBit64(keyBlock,bKey);  
    //生成子密钥  
    DES_MakeSubKeys(bKey,subKeys);  
      
    while(!feof(plain)){  
        //每次读8个字节，并返回成功读取的字节数  
        if((count = fread(plainBlock,sizeof(char),8,plain)) == 8){  
            DES_EncryptBlock(plainBlock,subKeys,cipherBlock);  
            fwrite(cipherBlock,sizeof(char),8,cipher);    
        }  
    }  
    if(count){  
        //填充  
        memset(plainBlock + count,'\0',7 - count);  
        //最后一个字符保存包括最后一个字符在内的所填充的字符数量  
        plainBlock[7] = 8 - count;  
        DES_EncryptBlock(plainBlock,subKeys,cipherBlock);  
        fwrite(cipherBlock,sizeof(char),8,cipher);  
    }  
    fclose(plain);  
    fclose(cipher);  
    return OK;  
}  
  
//解密文件  
int DES_Decrypt(char *cipherFile, char *keyStr,char *plainFile){  
    FILE *plain, *cipher;  
    int count,times = 0;  
    long fileLen;  
    char plainBlock[8],cipherBlock[8],keyBlock[8];  
    char bKey[64];  
    char subKeys[16][48];  
    if((cipher = fopen(cipherFile,"rb")) == NULL){  
        return CIPHER_FILE_OPEN_ERROR;  
    }  
    if((plain = fopen(plainFile,"wb")) == NULL){  
        return PLAIN_FILE_OPEN_ERROR;  
    }  
  
    //设置密钥  
    memcpy(keyBlock,keyStr,8);  
    //将密钥转换为二进制流  
    Char8ToBit64(keyBlock,bKey);  
    //生成子密钥  
    DES_MakeSubKeys(bKey,subKeys);  
  
    //取文件长度   
    fseek(cipher,0,SEEK_END);   //将文件指针置尾  
    fileLen = ftell(cipher);    //取文件指针当前位置  
    rewind(cipher);             //将文件指针重指向文件头  
    while(1){  
        //密文的字节数一定是8的整数倍  
        fread(cipherBlock,sizeof(char),8,cipher);  
        DES_DecryptBlock(cipherBlock,subKeys,plainBlock);                         
        times += 8;  
        if(times < fileLen){  
            fwrite(plainBlock,sizeof(char),8,plain);  
        }  
        else{  
            break;  
        }  
    }  
    //判断末尾是否被填充  
    if(plainBlock[7] < 8){  
        for(count = 8 - plainBlock[7]; count < 7; count++){  
            if(plainBlock[count] != '\0'){  
                break;  
            }  
        }  
    }     
    if(count == 7){//有填充  
        fwrite(plainBlock,sizeof(char),8 - plainBlock[7],plain);  
    }  
    else{//无填充  
        fwrite(plainBlock,sizeof(char),8,plain);  
    }  
  
    fclose(plain);  
    fclose(cipher);  
    return OK;  
}  
