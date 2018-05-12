#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/pkcs7.h>

#ifndef uchar
#define uchar unsigned char
#endif

int encrypt_data(const char *_key, const char *_vt, char *_raw_ptr,
        size_t _raw_size, char **_dst_buf, size_t *_dst_size)
{
    DES_key_schedule schedule;
    uchar key1[8];
    des_cblock *iv3;
    int pading;
    int len;
    size_t i, vt_size;
    char *mid_buf;

    memset(key1, 0, 8);
    memcpy(key1, _key, 8);
    DES_set_key_unchecked((const_DES_cblock *)&key1, &schedule);
    vt_size = strlen(_vt);
    iv3 = (des_cblock *)malloc(vt_size * sizeof(uchar));
    memcpy(iv3, _vt, vt_size);
    pading = 8 - (_raw_size % 8);
    *_dst_size = _raw_size + pading;
    mid_buf = (char *)malloc(*_dst_size);
    memcpy(mid_buf, _raw_ptr, _raw_size);
    for (i = _raw_size; i < *_dst_size; i++ )
    {
        mid_buf[i] = pading;
    }
    len = i;

    *_dst_buf = (char *)malloc(*_dst_size);
    DES_cbc_encrypt((const uchar *)mid_buf, (uchar *)*_dst_buf, *_dst_size, &schedule, iv3, DES_ENCRYPT);
    for (i = 0; i < len; i++)
    {
        printf("%02x", (uchar)*((*_dst_buf) + i));
    }
    printf("\n");
    free(iv3);
    free(mid_buf);

    return 1;
}

int decrypt_data(const char *_key, const char *_vt, char *_raw_ptr,
        size_t _raw_size, char **_dst_buf, size_t *_dst_size )
{
    DES_key_schedule schedule;
    uchar key1[8];
    des_cblock *iv3;
    int pading;
    size_t i, vt_size;
    char *mid_buf;

    memset(key1, 0, 8);
    memcpy(key1, _key, 8);
    DES_set_key_unchecked((const_DES_cblock *)&key1, &schedule);
    vt_size = strlen(_vt);
    iv3 = (des_cblock *)malloc(vt_size * sizeof(uchar));
    memcpy(iv3, _vt, vt_size);
    *_dst_buf = (char *)malloc(_raw_size);
    DES_cbc_encrypt((const uchar*)_raw_ptr, *_dst_buf, _raw_size, &schedule, iv3, DES_DECRYPT);
    free(iv3);

    return 1;
}

int main(int argc, char *argv[])
{
    char *_key = "jkl;!@#$";
    char *_vt = "asdf!@#$";
    char *_raw_ptr = NULL;
    size_t _raw_size;
    char *_dst_buf = NULL;
    size_t _dst_size;
    char *_final_buf = NULL;
    size_t _final_size;

    _raw_ptr = (char *)malloc(sizeof(char) * 6);
    memcpy(_raw_ptr, "hello", strlen("hello"));
    _raw_size = 6;
    encrypt_data(_key, _vt, _raw_ptr, _raw_size, &_dst_buf, &_dst_size);
    decrypt_data(_key, _vt, _dst_buf, _dst_size, &_final_buf, &_final_size);
    printf("final: %s\n", _final_buf);
    free(_dst_buf);

    return 0;
}

