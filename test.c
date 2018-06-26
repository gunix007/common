#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZLOG_CONF_PATH          "./zlog.conf"
#define FLASH_SRV_LOG_PATH      "\"/root/tftproot/log/server%M().log\""
#define TMP_SRV_LOG_PATH        "\"/tmp/log/server%M().log\""
#define FLASH_DEAMON_LOG_PATH   "\"/root/tftproot/log/deamon%M().log\""
#define TMP_DEAMON_LOG_PATH     "\"/tmp/log/deamon%M().log\""

typedef struct zlog_catarory_level_ {
    char commonLevel[8];
    char alarmLevel[8];
    char fifoLevel[8];
    char configLevel[8];
    char parseLevel[8];
    char mqttLevel[8];
    char hostLevel[8];
    char tunnelLevel[8];
    char rilLevel[8];
    char upgradeLevel[8];
    char weblmtLevel[8];
} zlog_catarory_level_t;

zlog_catarory_level_t zc_level;

int main(int argc, char *argv[])
{
    FILE *zconf_file = NULL;
    char *file_buf = NULL;
    int len = 0;

    file_buf = (char *)malloc(4096);
    if (!file_buf)
    {
        printf("malloc error\n");
        return -1;
    }

    strcpy(zc_level.commonLevel, "DEBUG");
    strcpy(zc_level.alarmLevel, "DEBUG");
    strcpy(zc_level.fifoLevel, "DEBUG");
    strcpy(zc_level.configLevel, "DEBUG");
    strcpy(zc_level.parseLevel, "DEBUG");
    strcpy(zc_level.mqttLevel, "DEBUG");
    strcpy(zc_level.hostLevel, "DEBUG");
    strcpy(zc_level.tunnelLevel, "DEBUG");
    strcpy(zc_level.rilLevel, "DEBUG");
    strcpy(zc_level.upgradeLevel, "DEBUG");
    strcpy(zc_level.weblmtLevel, "DEBUG");

    len += sprintf(file_buf + len, "[global]\n");
    len += sprintf(file_buf + len, "default format = \"%%d %%-6V [%%F:%%L] %%m%%n\"\n");
    len += sprintf(file_buf + len, "file perms = 644\n");
    len += sprintf(file_buf + len, "[rules]\n");
    len += sprintf(file_buf + len, "zc_common.%-16s%s\n", zc_level.commonLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_alarm.%-17s%s\n", zc_level.alarmLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_cfifo.%-17s%s\n", zc_level.fifoLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_gconfig.%-15s%s\n", zc_level.configLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_iparse.%-16s%s\n", zc_level.parseLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_mqtt.%-18s%s\n", zc_level.mqttLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_host.%-18s%s\n", zc_level.hostLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_rtunnel.%-15s%s\n", zc_level.tunnelLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_ril.%-19s%s\n", zc_level.rilLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_upgrade.%-15s%s\n", zc_level.upgradeLevel, TMP_SRV_LOG_PATH);
    len += sprintf(file_buf + len, "zc_weblmt.%-16s%s\n", zc_level.weblmtLevel, TMP_SRV_LOG_PATH);

    zconf_file = fopen(ZLOG_CONF_PATH, "wb");
    if (!zconf_file)
    {
        printf("error: failed create config file %s\n", ZLOG_CONF_PATH);
        free(file_buf);
        file_buf = NULL;
        return -1;
    }

    fwrite(file_buf, len, 1, zconf_file);
    fflush(zconf_file);
    free(file_buf);
    file_buf = NULL;
    fclose(zconf_file);
    system("sync");

    return 0;
}

