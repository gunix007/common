#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/statfs.h>

int get_disk_usage(char *path, float *usage)
{
    struct statfs vfs;

    if (-1 == statfs(path, &vfs))
    {
        perror("statfs error");
        return -1;
    }

    printf("size: %ld, used: %ld, available: %ld\n",
        vfs.f_bsize * vfs.f_blocks / 1024, vfs.f_bsize * (vfs.f_blocks - vfs.f_bfree) / 1024, vfs.f_bsize * vfs.f_bavail / 1024);
    *usage = (vfs.f_blocks - vfs.f_bfree) * 1.0 / (vfs.f_blocks - vfs.f_bfree + vfs.f_bavail);
    printf("usage: %f\n", *usage);

    return 0;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    float usage = 0.0;

    ret = get_disk_usage("/app", &usage);
    if (0 != ret)
    {
        printf("get app disk usage error\n");
    }
    ret = get_disk_usage("/factory", &usage);
    if (0 != ret)
    {
        printf("get factory disk usage error\n");
    }
    ret = get_disk_usage("/data", &usage);
    if (0 != ret)
    {
        printf("get data disk usage error\n");
    }
    ret = get_disk_usage("/test", &usage);
    if (0 != ret)
    {
        printf("get test disk usage error\n");
    }

    return 0;
}

