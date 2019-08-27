#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DEBUG_SRV_SERVER_PORT           50000
#define DEBUG_DEAMON_SERVER_PORT        50001
#define DEBUG_ENB_SERVER_PORT           16580
#define DEBUG_SYNC_HEADER               0x55AA
#define DEBUG_SYNC_FOOTER               0x55BB
#define DEBUG_PARAM_NUM                 4
#define DEBUG_CMD_MIN_NUM               4

#define eNB_2_UI_REPORT_UE_CATCH_INFO   102     /* 帧码上报 */
#define SOCKET_ERROR                    -1

typedef enum {
    READ_CMD = 0,
    SET_CMD,
    FAKE_IMSI_CMD,
    DEBUG_MAX_CMD,
} debug_cmd_t;

typedef enum {
    SRV = 0,
    CMCC_ENB_DEAMON,
    CMUC_ENB_DEAMON,
    CMTC_ENB_DEAMON,
    CMCC2_ENB_DEAMON,
    DEBUG_SERVER_MAX,
} debug_server_t;

typedef struct {
    int isp;
    int cmd_type;
    int msg_type;
    int param_num;
    int param[DEBUG_PARAM_NUM];
} dbg_main_input_param_t;

typedef struct {
    short sync_header;
    int isp;
    int cmd_type;
    int msg_type;
    int param_num;
    int dbg_param[DEBUG_PARAM_NUM];
    short sync_footer;
} dbg_cmd_req_t;

typedef struct {
    short sync_header;
    int isp;
    int cmd_type;
    int msg_type;
    int ret;
    short sync_footer;
} dbg_cmd_rsp_t;

typedef struct {
    char server_ip[16];
    int server_port;
} server_udp;

typedef struct {
    char imsi_str[18];      /* "460.11.4234567892" */
} imsi_t;

typedef struct {
    unsigned char tmsi[32];
} tmsi_t;

typedef struct {
    short sync_header;
    int msg_type;           /* eNB_2_UI_REPORT_UE_CATCH_INFO */
    imsi_t imsi;
    tmsi_t tmsi;            /* 暂不支持 */
    int ul_rx_power;        /* 暂不支持 */
    int rsrp;
    short sync_footer;
} ue_catch_report_t;

server_udp s_udp_server[DEBUG_SERVER_MAX] = {
    [SRV] = {"192.168.238.100", DEBUG_SRV_SERVER_PORT},
    [CMCC_ENB_DEAMON] = {"192.168.238.100", DEBUG_DEAMON_SERVER_PORT},
    [CMUC_ENB_DEAMON] = {"192.168.238.101", DEBUG_DEAMON_SERVER_PORT},
    [CMTC_ENB_DEAMON] = {"192.168.238.102", DEBUG_DEAMON_SERVER_PORT},
    [CMCC2_ENB_DEAMON] = {"192.168.238.103", DEBUG_DEAMON_SERVER_PORT},
};

static void build_req_msg(dbg_cmd_req_t *dbg_cmd_req, dbg_main_input_param_t *input_param)
{
    dbg_cmd_req->sync_header = DEBUG_SYNC_HEADER;
    if (input_param->isp > 0)
    {
        dbg_cmd_req->isp = input_param->isp - 1;
    }
    else
    {
        dbg_cmd_req->isp = 0;
    }
    dbg_cmd_req->cmd_type = input_param->cmd_type;
    dbg_cmd_req->msg_type = input_param->msg_type;
    dbg_cmd_req->param_num = input_param->param_num;
    memcpy(dbg_cmd_req->dbg_param,  input_param->param, sizeof(dbg_cmd_req->dbg_param));
    dbg_cmd_req->sync_footer = DEBUG_SYNC_FOOTER;

    return;
}

static void facke_imsi_catch_report(ue_catch_report_t *report, int isp)
{
    char plmn[5][3] = {"00", "00", "01", "11", "02"};
    char sub_card[5][8] = {" ", "cmcc", "cmuc", "cmtc", "cmcc2"};
    static int imsi_cnt[5] = {0} ;

    report->sync_header = 0x55AA;
    report->sync_footer = 0x66FF;
    sprintf(report->imsi.imsi_str, "460.%s.%010d", plmn[isp], rand());
    report->msg_type = eNB_2_UI_REPORT_UE_CATCH_INFO;
    report->rsrp = 0;
    report->ul_rx_power = 0;
    imsi_cnt[isp]++;

    printf("facke_imsi_catch_report sub_card: %s, imsi: %s, cnt: %d\n", sub_card[isp], report->imsi.imsi_str, imsi_cnt[isp]);
    return;
}

static void parse_rsp_msg(dbg_cmd_rsp_t *dbg_cmd_rsp)
{
    printf("header: %x, footer: %x, isp: %d, cmd: %d, msg: %d, ret: %d\n",
        dbg_cmd_rsp->sync_header, dbg_cmd_rsp->sync_footer, dbg_cmd_rsp->isp, dbg_cmd_rsp->cmd_type, dbg_cmd_rsp->msg_type, dbg_cmd_rsp->ret);
    return;
}

static int parse_main_input_param(int argc, char *argv[], dbg_main_input_param_t *input_param)
{
    int i;

    if ((argc > (DEBUG_PARAM_NUM + DEBUG_CMD_MIN_NUM)) || (argc < DEBUG_CMD_MIN_NUM))
    {
        printf("argc num: %d err\n", argc);
        printf("format: ddbg isp(srv: 0, deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fake_imsi) msg_type param1 param2 param3 param4\n");
        return -1;
    }

    input_param->isp = atoi(argv[1]);
    if (input_param->isp >= DEBUG_SERVER_MAX)
    {
        printf("isp: %d err\n", input_param->isp);
        printf("format: ddbg isp(srv: 0, deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fake_imsi) msg_type param1 param2 param3 param4\n");
        return -2;
    }

    input_param->cmd_type = atoi(argv[2]);
    if (input_param->cmd_type >= DEBUG_MAX_CMD)
    {
        printf("cmd_type: %d err\n", input_param->cmd_type);
        printf("format: ddbg isp(srv: 0, deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fake_imsi) msg_type param1 param2 param3 param4\n");
        return -3;
    }

    input_param->msg_type = atoi(argv[3]);
    input_param->param_num = 0;
    for (i = 4; i < argc; i++)
    {
        input_param->param[i - 4] = atoi(argv[i]);
        input_param->param_num++;
    }

    printf("argc: %d, isp: %d, server_ip: %s, port: %d, cmd_type: %d, msg_type: %d, num: %d, param: %d %d %d %d\n",
        argc, input_param->isp, s_udp_server[input_param->isp].server_ip, s_udp_server[input_param->isp].server_port,
        input_param->cmd_type, input_param->msg_type, input_param->param_num,
        input_param->param[0], input_param->param[1], input_param->param[2], input_param->param[3]);

    return 0;
}

static void send_request_msg(dbg_main_input_param_t *input_param, int sockfd, int addr_len)
{
    struct sockaddr_in addr;
    int len = 0;
    dbg_cmd_req_t dbg_cmd_req;
    char buffer[256];
    int ret = 0;

    /* 填写 sockaddr_in */
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_udp_server[input_param->isp].server_port);
    addr.sin_addr.s_addr = inet_addr(s_udp_server[input_param->isp].server_ip);
    printf("DbgCmd port: %d, isp: %d, msg_type: %d\n", addr.sin_port, input_param->isp, input_param->msg_type);

    /* 填写请求消息内容 */
    memset(&dbg_cmd_req, 0, sizeof(dbg_cmd_req_t));
    build_req_msg(&dbg_cmd_req, input_param);

    /* 将请求消息传送给 server 端*/
    ret = sendto(sockfd, &dbg_cmd_req, sizeof(dbg_cmd_req_t), 0, (struct sockaddr *)&addr, addr_len);
    if (SOCKET_ERROR == ret)
    {
        printf("send_request_msg ret: %d, errno: %s, cmd_type: %d, ip: %s\n",
            ret, strerror(errno), input_param->cmd_type, s_udp_server[input_param->isp].server_ip);
        return;
    }

    usleep(500000);
    /* 接收 server 端返回的字符串 */
    bzero(buffer, sizeof(buffer));
    len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, (socklen_t *)&addr_len);
    printf("Receive from server: %s\n", buffer);
    parse_rsp_msg((dbg_cmd_rsp_t *)buffer);

    return;
}

static void send_fake_imsi(dbg_main_input_param_t *input_param, int sockfd, int addr_len)
{
    struct sockaddr_in addr;
    ue_catch_report_t report;
    int start;
    int end;
    int i;
    int ret = 0;

    /* 填写 sockaddr_in */
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEBUG_ENB_SERVER_PORT);
    printf("FakeImsiRpt port: %d, isp: %d, msg_type: %d\n", addr.sin_port, input_param->isp, input_param->msg_type);
    while (1)
    {
        /* 将请求消息传送给 server 端 */
        if (SRV == input_param->isp)
        {
            start = CMCC_ENB_DEAMON;
            end = DEBUG_SERVER_MAX;
        }
        else
        {
            start = input_param->isp;
            end = input_param->isp + 1;
        }

        for (i = start; i < end; i++)
        {
            /* 填写 Fake Imsi */
            memset(&report, 0, sizeof(ue_catch_report_t));
            facke_imsi_catch_report(&report, i);
            addr.sin_addr.s_addr = inet_addr(s_udp_server[i].server_ip);
            ret = sendto(sockfd, &report, sizeof(ue_catch_report_t), 0, (struct sockaddr *)&addr, addr_len);
            if (SOCKET_ERROR == ret)
            {
                printf("FakeImsiRpt ret: %d, errno: %s, ip: %s\n", ret, strerror(errno), s_udp_server[i].server_ip);
                break;
            }
        }

        if (input_param->msg_type == 0)
        {
            break;
        }

        if (0 == input_param->param[0])
        {
            usleep(20000);
        }
        else
        {
            usleep(1000 * input_param->param[0]);
        }
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int sockfd;
    int addr_len = sizeof(struct sockaddr_in);
    dbg_main_input_param_t input_param;
    int ret = 0;

    /* 解析输入参数 */
    memset(&input_param, 0, sizeof(dbg_main_input_param_t));
    ret = parse_main_input_param(argc, argv, &input_param);
    if (ret < 0)
    {
        printf("ParseParam ret: %d err\n", ret);
        exit(1);
    }

    /* 建立 socket，注意必须是 SOCK_DGRAM */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket err\n");
        exit(2);
    }

    /* 发送消息 */
    switch (input_param.cmd_type)
    {
    case READ_CMD:
    case SET_CMD:
        send_request_msg(&input_param, sockfd, addr_len);
        break;

    case FAKE_IMSI_CMD:
        send_fake_imsi(&input_param, sockfd, addr_len);
        break;

    default:
        break;
    }

    return 0;
}

