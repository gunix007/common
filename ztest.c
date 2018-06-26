#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    /*
     * linebuffer：读取文件中的一行存入的缓冲
     * buffer1：一行中第一个字段的存入缓冲
     * buffer2：一行中第二个字段的存入缓冲
     * buffer3：一行中第三个字段的存入缓冲
     */
    char linebuffer[128] = {0};
    char buffer1[128] = {0};
    char buffer2[128] = {0};
    char buffer3[128] = {0};
    int res;

    /* zlog.conf是文件名，r+代表可以读写文件 */
    FILE *fp = fopen("zlog.conf", "r+");
    if (fp == NULL)
    {
        printf("open error");
        return -1;
    }

    while(fgets(linebuffer, 128, fp))
    {
        /*
         * buffer1=wireless.1.current_state
         * buffer2=1
         */
        sscanf(linebuffer, "%[a-z_].%[A-Z]", buffer1, buffer2);
        if (!strcmp("zc_common", buffer1))
        {
            printf("%s\n", buffer2);
        }
    }

    return 0;
}

