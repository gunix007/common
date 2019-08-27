#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>

typedef struct {
    int MCC[3];
    int MNC[2];
} Plmn_t;

Plmn_t plmn[5];

typedef struct {
	char plmn[8];
} plmnStr_t;

plmnStr_t fplmn = {
    .plmn = "460.01"
};

int main(void)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        plmn[i].MCC[0] = 4;
        plmn[i].MCC[1] = 6;
        plmn[i].MCC[2] = 0;
    }

    sscanf(fplmn.plmn, "460.%1d%1d", &plmn[0].MNC[0], &plmn[0].MNC[1]);
    printf("MNC[0] = %d, MNC[1] = %d\n", plmn[0].MNC[0], plmn[0].MNC[1]);
#if 0
    if (strcmp(fplmn.plmn, "460.01") == 0)
    {
        plmn[i].MNC[0] = 0;
        plmn[i].MNC[1] = 1;
    }
    else if (strcmp(fplmn.plmn, "460.11") == 0)
    {
        plmn[i].MNC[0] = 1;
        plmn[i].MNC[1] = 1;
    }
    else
    {
        plmn[i].MNC[0] = 0;
        plmn[i].MNC[1] = 0;
    }
#endif
    return 0;
}





