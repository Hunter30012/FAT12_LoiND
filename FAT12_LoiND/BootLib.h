#ifndef _BOOTLIB_
#define _BOOTLIB_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
/***************************
Define
****************************/

#define BOOT_SECTOR_SIZE 512

#pragma pack(push) /* push current alignment to stack */
#pragma pack(1)
typedef struct 
{
    uint8_t u8BootJumpInstruction[3];  /* 0 */
    uint8_t u8OemIdentifier[8];        /*3 */
    uint16_t u16BytesPerSector;        /*11 */
    uint8_t u8SectorsPerCluster;       /*13 */  
    uint16_t u16ReservedSectors;       /*14 */
    uint8_t u8FatCount;               /*16 */
    uint16_t u16RootEntryCount;       /*17 */    /* DirEntry count  Kich thuoc = ... * 32  kich thuuoc DIr */
    uint16_t u16TotalSectors;          /*19*/           /* total 4 regions */
    uint8_t u8MediaDescriptorType;     /*21*/  
    uint16_t u16SectorsPerFat;        /*22*/
    uint16_t u16SectorsPerTrack;      /*24*/
    uint16_t u16Heads;                 /*26*/
    uint32_t u32HiddenSectors;         /*28*/
    uint32_t u32LargeSectorCount;      /*32*/

    // extended boot record  FAT12
    uint8_t u8DriveNumber;
    uint8_t u8Reserved;
    uint8_t u8BootSignature;
    uint32_t u32VolumeId;          /* serial number, value doesn't matter */
    uint8_t VolumeLabel[11];       /* 11 bytes, padded with spaces */
    uint8_t SystemId[8];           /*54*/

}BootSector;
#pragma pack(pop)


#pragma pack(push) /* push current alignment to stack */
#pragma pack(1)
typedef struct 
{
    uint8_t FileName[8];
    uint8_t Ext[3];
    uint8_t u8Attributes;            
    uint8_t u8Reserved;              
    uint8_t u8CreatedTimeTenths;     
    uint16_t u16CreatedTime;         
    uint16_t u16CreatedDate;         
    uint16_t u16AccessedDate;
    uint16_t u16FirstClusterHigh;    
    uint16_t u16ModifiedTime;
    uint16_t u16ModifiedDate;
    uint16_t u16FirstClusterLow;     /* vi tri cluster doi voi FAt12/16  */
    uint32_t u32Size;
}RootDirectory;
#pragma pack(pop)

/***************************************************
Function:

void Read_FirstSector(FILE *f);
@Doc Sector dau tien (BootSector)

void Read_Sectors(FILE* f, uint16_t u16offset_sector, uint32_t u16Num,void* bufferOut);
@u16offset_sector: vi tri cua Sector bat dau doc
@u16Num: So sector can doc
@bufferOut: Mang luu cac Sectors can doc

void Print_BootSector_Infor();
@In ra cac thong tin cua BootSector

******************************************************/

void Read_FirstSector(FILE *f);
void Read_Sectors(FILE* f, uint16_t u16offset_sector, uint32_t u16Num,void* bufferOut);
void Print_BootSector_Infor();

#endif
