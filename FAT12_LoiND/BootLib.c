#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "BootLib.h"

extern BootSector g_BootSector;
extern uint16_t g_DataStart_sector;


void Read_FirstSector(FILE *f)
{
    fread(&g_BootSector, BOOT_SECTOR_SIZE, 1, f);
}

void Read_Sectors(FILE* f, uint16_t u16offset_sector, uint32_t u16Num,void* bufferOut)  /*Num: so sector*/
/*offset theo sector*/
{

    fseek(f, u16offset_sector * g_BootSector.u16BytesPerSector, SEEK_SET);
    fread(bufferOut, g_BootSector.u16BytesPerSector, u16Num, f);
}

void Print_BootSector_Infor()
{
    printf("Bytes per sector: %d\n", g_BootSector.u16BytesPerSector);
    printf("Sectors per cluster: %d\n", g_BootSector.u8SectorsPerCluster);
    printf("Reserved sector count: %d\n", g_BootSector.u16ReservedSectors);
    printf("Number of FATs: %d\n", g_BootSector.u8FatCount);
    printf("Total sectors: %d\n", g_BootSector.u16TotalSectors);
    printf("Sectors per FAT: %d\n", g_BootSector.u16SectorsPerFat);
    printf("Root directory entry count: %d\n", g_BootSector.u16RootEntryCount);
}
