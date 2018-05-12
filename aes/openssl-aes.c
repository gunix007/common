#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static unsigned char srcdata[150*10000] = {0};

int main(int argc, char **argv)
{
    int i;
    int fd;
    int readlen;
    size_t len;
    unsigned char *output = NULL;
    DES_cblock key;
    unsigned char ivec[8] = {0};
    DES_key_schedule key_schedule;

    fd = open("dazhan.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("open failed!\n");
        return errno;
    }

    //readlen = read(fd, srcdata, sizeof(srcdata));
    readlen = read(fd, srcdata, 10);
    if (readlen < 0)
    {
        printf("read imis data error\n");
        close(fd);
        return errno;
    }
    close(fd);
    printf("imse data:\n%s\n", srcdata);

    memcpy(key, "password", 8);
    DES_set_key_unchecked(&key, &key_schedule);

    len = (readlen + 7) / 8 * 8;
    output = malloc(len + 1);
    memset(ivec, 0, sizeof(ivec));
    printf("\n***** sizeof(ivec): %lu, strlen(srcdata): %d, readlen: %d, len: %d *****\n", sizeof(ivec), strlen(srcdata), readlen, len);
#if 1
    memcpy(ivec, "7654321", sizeof(ivec));
    for (i = 0; i < sizeof(ivec); i++)
    {
        ivec[i] = 8 - i;
    }
#endif

    DES_ncbc_encrypt(srcdata, output, strlen(srcdata), &key_schedule, &ivec, DES_ENCRYPT);

    for (i = 0; i < len; ++i)
        printf("%02x", output[i]);
    printf("\n");

    memset((char *)&ivec, 0, sizeof(ivec));
#if 1
    memcpy(ivec, "7654321", sizeof(ivec));
    for (i = 0; i < sizeof(ivec); i++)
    {
        ivec[i] = 8 - i;
    }
#endif

    DES_ncbc_encrypt(output, srcdata, len, &key_schedule, &ivec, DES_DECRYPT);
    printf("\n%s\n", srcdata);
    free(output);

    return EXIT_SUCCESS;
}

