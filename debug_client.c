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

#define SOCKET_ERROR                    -1
#define ENB_2_UI_REPORT_UE_CATCH_INFO   102     /* 帧码上报 */

typedef enum {
    CMD_READ_PARM = 0,
    CMD_SET_PARM,
    CMD_FAKE_IMSI,
    CMD_MAX,
} debug_cmd_t;

typedef enum {
    SRV = 0,
    CMCC_ENB_DEAMON,
    CMUC_ENB_DEAMON,
    CMTC_ENB_DEAMON,
    CMCC2_ENB_DEAMON,
    SERVER_PORT_MAX,
} server_port_t;

typedef struct {
    int isp;
    int cmd_type;
    int msg_type;
    int parm_num;
    int input_parms[DEBUG_PARAM_NUM];
} input_parm_t;

typedef struct {
    short sync_header;
    int isp;
    int cmd_type;
    int msg_type;
    int parm_num;
    int dbg_parms[DEBUG_PARAM_NUM];
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
    char ip[16];
    int port;
} server_addr_t;

typedef struct {
    char imsi[18];          /* "46.011.4234567892" */
} imsi_t;

typedef struct {
    unsigned char tmsi[32];
} tmsi_t;

typedef struct {
    short sync_header;
    int msg_type;           /* ENB_2_UI_REPORT_UE_CATCH_INFO */
    imsi_t imsi;
    tmsi_t tmsi;            /* 暂不支持 */
    int ul_rx_power;        /* 暂不支持 */
    int rsrp;
    short sync_footer;
} ue_catch_report_t;

server_addr_t s_server_addrs[SERVER_PORT_MAX] = {
    [SRV]               = {"192.168.238.100", DEBUG_SRV_SERVER_PORT},
    [CMCC_ENB_DEAMON]   = {"192.168.238.100", DEBUG_DEAMON_SERVER_PORT},
    [CMUC_ENB_DEAMON]   = {"192.168.238.101", DEBUG_DEAMON_SERVER_PORT},
    [CMTC_ENB_DEAMON]   = {"192.168.238.102", DEBUG_DEAMON_SERVER_PORT},
    [CMCC2_ENB_DEAMON]  = {"192.168.238.103", DEBUG_DEAMON_SERVER_PORT},
};

static void build_req_msg(dbg_cmd_req_t *req, input_parm_t *parm)
{
    req->sync_header = DEBUG_SYNC_HEADER;
    if (parm->isp > 0)
    {
        req->isp = parm->isp - 1;
    }
    else
    {
        req->isp = 0;
    }
    req->cmd_type = parm->cmd_type;
    req->msg_type = parm->msg_type;
    req->parm_num = parm->parm_num;
    memcpy(req->dbg_parms,  parm->input_parms, sizeof(req->dbg_parms));
    req->sync_footer = DEBUG_SYNC_FOOTER;

    return;
}

static void fake_imsi_catch_report(ue_catch_report_t *report, int isp)
{
    static int imsi_cnt[5] = {0} ;
    char plmn[5][3] = {"00", "00", "01", "11", "02"};
    char subcard[5][8] = {" ", "cmcc", "cmuc", "cmtc", "cmcc2"};

    report->sync_header = DEBUG_SYNC_HEADER;
    report->sync_footer = DEBUG_SYNC_FOOTER;
    sprintf(report->imsi.imsi, "460.%s.%010d", plmn[isp], rand());
    report->msg_type = ENB_2_UI_REPORT_UE_CATCH_INFO;
    report->rsrp = 0;
    report->ul_rx_power = 0;
    imsi_cnt[isp]++;
    printf("fake_imsi_catch_report subcard: %s, imsi: %s, cnt: %d\n", subcard[isp], report->imsi.imsi, imsi_cnt[isp]);

    return;
}

static void parse_rsp_msg(dbg_cmd_rsp_t *rsp)
{
    printf("header: %x, footer: %x, isp: %d, cmd: %d, msg: %d, ret: %d\n",
        rsp->sync_header, rsp->sync_footer, rsp->isp, rsp->cmd_type, rsp->msg_type, rsp->ret);
    return;
}

static int parse_input_parms(int argc, char *argv[], input_parm_t *parms)
{
    int i;
    if ((argc > (DEBUG_PARAM_NUM + DEBUG_CMD_MIN_NUM))
        || (argc < DEBUG_CMD_MIN_NUM))
    {
        printf("argc num: %d err\n", argc);
        printf("format: ddbg isp(0: srv deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fakeimsi) msg_type param1 param2 param3 param4\n");
        return -1;
    }

    parms->isp = atoi(argv[1]);
    if (parms->isp >= SERVER_PORT_MAX)
    {
        printf("isp: %d err\n", parms->isp);
        printf("format: ddbg isp(0: srv deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fakeimsi) msg_type param1 param2 param3 param4\n");
        return -2;
    }

    parms->cmd_type = atoi(argv[2]);
    if (parms->cmd_type >= CMD_MAX)
    {
        printf("cmd_type: %d err\n", parms->cmd_type);
        printf("format: ddbg isp(0: srv deamon: /1/2/3/4) cmd_type(0: read 1: set 2: fakeimsi) msg_type param1 param2 param3 param4\n");
        return -3;
    }

    parms->msg_type = atoi(argv[3]);
    parms->parm_num = 0;
    for (i = 4; i < argc; i++)
    {
        parms->input_parms[i - 4] = atoi(argv[i]);
        parms->parm_num++;
    }

    printf("argc: %d, isp: %d, ip: %s, port: %d, cmd_type: %d, msg_type: %d, num: %d, param: %d %d %d %d\n",
        argc, parms->isp, s_server_addrs[parms->isp].ip, s_server_addrs[parms->isp].port, 
        parms->cmd_type, parms->msg_type, parms->parm_num,
        parms->input_parms[0], parms->input_parms[1], parms->input_parms[2], parms->input_parms[3]);

    return 0;
}

static void send_req_msg(input_parm_t *parms, int sockfd, int addr_len)
{
    struct sockaddr_in addr;
    int len = 0;
    dbg_cmd_req_t req;
    char buffer[256];
    int ret = 0;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(s_server_addrs[parms->isp].port);
    addr.sin_addr.s_addr = inet_addr(s_server_addrs[parms->isp].ip);
    printf("dbg_cmd port: %d, isp: %d, msg_type: %d\n", addr.sin_port, parms->isp, parms->msg_type);

    /* 填写请求消息内容 */
    memset(&req, 0, sizeof(dbg_cmd_req_t));
    build_req_msg(&req, parms);

    /* 将请求消息传送给 server 端 */
    ret = sendto(sockfd, &req, sizeof(dbg_cmd_req_t), 0, (struct sockaddr *)&addr, addr_len);
    if (SOCKET_ERROR == ret)
    {
        printf("send_req_msg ret: %d, errno: %s, cmd_type: %d, ip: %s\n",
            ret, strerror(errno), parms->cmd_type, s_server_addrs[parms->isp].ip);
        return;
    }

    usleep(500000);
    /* 接收 server 端返回的字符串 */
    bzero(buffer, sizeof(buffer));
    len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, (socklen_t *)&addr_len);
    printf("receive from server: %s\n", buffer);
    parse_rsp_msg((dbg_cmd_rsp_t *)buffer);

    return;
}

static void send_fake_imsi(input_parm_t *parms, int sockfd, int addr_len)
{
    int i;
    int start;
    int end;
    int ret = 0;
    struct sockaddr_in addr;
    ue_catch_report_t report;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEBUG_ENB_SERVER_PORT);
    printf("fake_imsi_report port: %d, isp: %d, msg_type: %d\n", addr.sin_port, parms->isp, parms->msg_type);
    while (1)
    {
        /* 将请求消息传送给 server 端 */
        if (SRV == parms->isp)
        {
            start = CMCC_ENB_DEAMON;
            end = SERVER_PORT_MAX;
        }
        else
        {
            start = parms->isp;
            end = parms->isp + 1;
        }

        for (i = start; i < end; i++)
        {
            /* 填写 fake imsi */
            memset(&report, 0, sizeof(ue_catch_report_t));
            fake_imsi_catch_report(&report, i);
            addr.sin_addr.s_addr = inet_addr(s_server_addrs[i].ip);
            ret = sendto(sockfd, &report, sizeof(ue_catch_report_t), 0, (struct sockaddr *)&addr, addr_len);
            if (SOCKET_ERROR == ret)
            {
                printf("fake_imsi_report ret: %d, errno: %s, ip: %s\n", ret, strerror(errno), s_server_addrs[i].ip);
                break;
            }
        }

        if (parms->msg_type == 0)
        {
            break;
        }

        if (0 == parms->input_parms[0])
        {
            usleep(20000);
        }
        else
        {
            usleep(1000 * parms->input_parms[0]);
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int sockfd;
    struct sockaddr_in addr;
    int addr_len = sizeof(struct sockaddr_in);
    input_parm_t parms;

    /* 解析输入参数 */
    memset(&parms, 0, sizeof(input_parm_t));
    ret = parse_input_parms(argc, argv, &parms);
    if (ret < 0)
    {
        printf("parse_input_parms ret: %d err\n", ret);
        exit(1);
    }

    /* 建立 socket，注意必须是 SOCK_DGRAM */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket err\n");
        exit(2);
    }

    /* 发送消息*/
    switch(parms.cmd_type)
    {
    case CMD_READ_PARM:
    case CMD_SET_PARM:
        send_req_msg(&parms, sockfd, addr_len);
        break;

    case CMD_FAKE_IMSI:
        send_fake_imsi(&parms, sockfd, addr_len);
        break;

    default:
        break;
    }

    return 0;
}

