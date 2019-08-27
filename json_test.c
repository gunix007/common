#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

void ParseJSON(cJSON *cont)
{
    char *p = NULL;

    p = cJSON_Print(cont);
    printf("parse: \n%s\n\n", p);
    free(p);
    p = NULL;

    return;
}

void DelJSON(cJSON *cont)
{
    if (NULL != cont)
    {
        cJSON_Delete(cont);
        cont = NULL;
    }
    return;
}

int main()
{
    int i;
    int plmnNum;
    FILE *fp;
    long len;
    char *content;
    cJSON *root;
    cJSON *isp_arr;
    cJSON *item;
    cJSON *plmnItem;
    cJSON *freqItem;
    cJSON *cfreqItem;
    cJSON *commonP;
    cJSON *plmnArray;
    cJSON *testArray;

    fp = fopen("./json_test.txt", "rw");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    content = (char *)malloc(len + 1);
    fread(content, 1, len, fp);
    fclose(fp);

    root = cJSON_Parse(content);
    if (!root)
    {
        printf("json error\n");
        return 0;
    }

    ParseJSON(root);

    isp_arr = cJSON_GetObjectItem(root, "opDeviceParameter");
    if (NULL == isp_arr)
    {
        printf("opDeviceParameter is null\n");
        return 0;
    }
    freqItem = cJSON_GetObjectItem(isp_arr, "Frequency");
    plmnItem = cJSON_GetObjectItem(freqItem, "plmn");
    plmnNum = cJSON_GetArraySize(plmnItem);

    for (i = 0; i < plmnNum; i++)
    {
        item = cJSON_GetArrayItem(plmnItem, i);
        printf("%s\n", item->valuestring);
    }

    DelJSON(root);

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "code", "000000");
    cJSON_AddStringToObject(root, "msg", "Success");
    cJSON_AddItemToObject(root, "opDeviceParameter", item = cJSON_CreateObject());
    cJSON_AddStringToObject(item, "operationType", "M");
    cJSON_AddItemToObject(item, "commonParameter", commonP = cJSON_CreateObject());
    cJSON_AddNumberToObject(commonP, "tacPeriod", 120);
    cJSON_AddNumberToObject(commonP, "bandWidth", 3);
    cJSON_AddNumberToObject(commonP, "tac", 238);
    cJSON_AddNumberToObject(commonP, "redirected_earfcn", 38400);
    cJSON_AddNumberToObject(commonP, "syncMode", 0);
    cJSON_AddNumberToObject(commonP, "imsiReportInterval", 0);
    cJSON_AddNumberToObject(commonP, "radioSwitch", 1);
    cJSON_AddItemToObject(item, "Frequency", cfreqItem = cJSON_CreateObject());
    cJSON_AddNumberToObject(cfreqItem, "upFrequency", 38950);
    cJSON_AddNumberToObject(cfreqItem, "downFrequency", 38950);
    cJSON_AddNumberToObject(cfreqItem, "priority", 0);
    cJSON_AddNumberToObject(cfreqItem, "rsrp", 0);
    cJSON_AddNumberToObject(cfreqItem, "pci", 5);
    char plmns[5][7] = {0};
    char *plmnsptr[20];
    int inttestarray[20] = {-1, 0, 1, -2, -3};
    int intNum = 5;
    char *jsonStr = NULL;
    plmnNum = 3;

    for (i = 0; i < plmnNum; i++)
    {
        plmnsptr[i] = plmns[i];
        // strcpy(plmns[i], "460.01");
        sprintf(plmns[i], "460.0%d", i);
    }
    cJSON_AddItemToObject(cfreqItem, "plmn", plmnArray = cJSON_CreateStringArray((const char **)plmnsptr, plmnNum));
    // cJSON_AddItemToObject(cfreqItem, "array_test", testArray = cJSON_CreateIntArray((const int *)inttestarray, intNum));
    cJSON_AddItemToObject(cfreqItem, "array_test", testArray = cJSON_CreateIntArray(inttestarray, intNum));
    cJSON_AddNumberToObject(cfreqItem, "frameOffset", 0);
    cJSON_AddNumberToObject(cfreqItem, "powerLevel", 0);
    jsonStr = cJSON_Print(root);
    printf("jsonStr: \n%s\n", jsonStr);
    DelJSON(root);
    free(jsonStr);

    return 0;
}

