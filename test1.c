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
     */
    char linebuffer[128] = {0};
    char buffer1[128] = {0};
    char buffer2[128] = {0};

    int line_len = 0;
    int len = 0;
    int res;

    /* cc.cfg是文件名，r+代表可以读写文件 */
    FILE *fp = fopen("cc.cfg", "r+");
    if (fp == NULL)
    {
        printf("open error");
        return -1;
    }

    while(fgets(linebuffer, 128, fp))
    {
        line_len = strlen(linebuffer);
        len += line_len;
        /*
         * buffer1=wireless.1.current_state
         * buffer2=1
         */
        sscanf(linebuffer, "%[^=]=%[^=]", buffer1, buffer2);
        if (!strcmp("wireless.1.current_state", buffer1))
        {
            /* 由于已经找到所需要写的位置，所以需要写位置的“头” */
            len -= strlen(linebuffer);
            /* 实现文件位置的偏移，为写文件做准备 */
            res = fseek(fp, len, SEEK_SET);
            if (res < 0)
            {
                perror("fseek");
                return -1;
            }
            strcpy(buffer2, "=0");
            /* strcat(buffer1, "="); */
            strcat(buffer1, buffer2);
            printf("%d\n", strlen(buffer1));
            /* 写文件，存入所需的内容 */
            fprintf(fp, "%s", buffer1);
            fclose(fp);
            return;
        }
    }

    return 0;
}

