
#include "BootLib.h"
#include "RootLib.h"
#include "InOut.h"
#include "FAT_Table.h"
#include <math.h>
extern BootSector g_BootSector;
extern uint16_t g_DataStart_sector;

uint16_t Find_Next_Cluster(uint16_t nowPois, FILE *fp)
{
    uint16_t nextPois;
    uint8_t *fat;
    uint16_t i;
    uint16_t du;
    uint16_t offset;
    i = ceil((float)nowPois/341);
    /* j = ceil((float)nowPois/341); */
    du = i % 3;
    if( du == 0 )
    {
        i = i/3 - 1;
    }
    else
    {
        i = ( i - du)/3;
    }
    offset = 3*i + 1;
    /* if( (nowPois - 341)%1024 == 0 )
    {
        fat = (uint8_t *)malloc(2 * g_BootSector.u16BytesPerSector);
        Read_Sectors(fp, j, 2, fat);
        nextPois = READ(341);
    }
    else if( (nowPois - 682)%1024 == 0 )
    {
        fat = (uint8_t *)malloc(2 * g_BootSector.u16BytesPerSector);
        Read_Sectors(fp, j, 2, fat);
        nextPois = READ(341);
    }
    else
    {
        fat = (uint8_t *)malloc( 1 * g_BootSector.u16BytesPerSector);
        temp = (uint8_t *)malloc( 1 * g_BootSector.u16BytesPerSector);
        Read_Sectors(fp, j, 1, temp);
        nowPois = nowPois - 341 * (j - 1);
        if( j % 3 == 0)
        {
            memcpy(fat, &temp[2], 510);
            nextPois = READ(nowPois);
        }
        else if( j % 2 == 0)
        {
            memcpy(fat, &temp[1], 510);
            nextPois = READ(nowPois);
        }
        else
        {
            memcpy(fat, &temp[0], 510);
            nextPois = READ(nowPois);
        }
    } */
    fat = (uint8_t *)malloc(3 * g_BootSector.u16BytesPerSector);
    Read_Sectors(fp, offset, 3, fat);
    nowPois = nowPois - 1024 * i;
    nextPois = READ(nowPois);
    free(fat);
    /*free(temp);*/
    return nextPois;
}
