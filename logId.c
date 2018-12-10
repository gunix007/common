#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int GetLogId(int *logId)
{
    int ret = 0;
    FILE *fp = NULL;

    if (0 == access("logId", F_OK))
    {
        fp = fopen("logId", "r+");
        if (NULL == fp)
        {
            printf("open logId failed, errno: %d\n", errno);
            return -1;
        }

        ret = fscanf(fp, "%d", logId);
        if (1 != ret)
        {
            printf("read logId error, ret: %d\n", ret);
            fclose(fp);
            return -2;
        }
        printf("logId: %d\n", *logId);
        rewind(fp);
        ret = fprintf(fp, "%d", *logId % 5 + 1);
        if (1 != ret)
        {
            printf("write logId error, ret: %d\n", ret);
            fclose(fp);
            return -3;
        }
        fclose(fp);
    }
    else
    {
        fp = fopen("logId", "w");
        if (NULL == fp)
        {
            printf("create logid failed, errno: %d\n", errno);
            return -4;
        }
        *logId = 1;
        ret = fprintf(fp, "%d", *logId % 5 + 1);
        if (1 != ret)
        {
            printf("write logId error, ret: %d\n", ret);
            fclose(fp);
            return -5;
        }
        fclose(fp);
    }

    return 0;
}

int main(void)
{
    int ret = 0;
    int logId = 0;
    FILE *fp = NULL;

    ret = GetLogId(&logId);
    if (0 != ret)
    {
        printf("GetLogId error, ret: %d\n", ret);
    }

    return 0;
}

