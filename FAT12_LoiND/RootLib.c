#include "BootLib.h"
#include "RootLib.h"

extern BootSector g_BootSector;
extern uint16_t g_DataStart_sector;

void Read_Dir(FILE *f , RootDirectory **arr)
{
    uint16_t u16Root_offset;
    uint16_t u16Size;
    uint16_t u16Numsectors;
    u16Size = 32 * g_BootSector.u16RootEntryCount;
    u16Root_offset = g_BootSector.u16ReservedSectors + g_BootSector.u8FatCount * g_BootSector.u16SectorsPerFat;
    u16Numsectors = u16Size / g_BootSector.u16BytesPerSector;
    if(u16Size % g_BootSector.u16BytesPerSector > 0)
    {
        u16Numsectors++;
    }
    g_DataStart_sector = u16Root_offset + u16Numsectors; 
    *arr = (RootDirectory *)malloc(32 * g_BootSector.u16RootEntryCount);
    Read_Sectors(f , u16Root_offset, u16Numsectors, *arr);
}
void Cre_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s)
{
    *h = Entry.u16CreatedTime >> 11;
    *m = ((Entry.u16CreatedTime) & 0x3F0) >> 5;
    *s = (Entry.u16CreatedTime & 0x1F) * 2;
}

void Cre_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day)
{
    *year = (Entry.u16CreatedDate >> 9) + 1980;
    *month = (Entry.u16CreatedDate & 0x1E0) >> 5;
    *day = Entry.u16CreatedDate & 0x1F;
}

void Modf_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s)
{
    *h = Entry.u16ModifiedTime >> 11;
    *m = ((Entry.u16ModifiedTime) & 0x3F0) >> 5;
    *s = (Entry.u16ModifiedTime & 0x1F) * 2;
}

void Modf_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day)
{
    *year = (Entry.u16ModifiedDate >> 9) + 1980;
    *month = (Entry.u16ModifiedDate & 0x1E0) >> 5;
    *day = Entry.u16ModifiedDate & 0x1F;
}
void Display(RootDirectory *Root_Dir, FILE *f)
{
    uint32_t size; /* size cua folder */
    uint8_t u8day;
    uint8_t u8mon;
    uint16_t u16year;
    uint8_t u8h;
    uint8_t u8m;
    uint8_t u8s;
    int j;
    int i;
    uint8_t flag;
    printf("Ten File       Size      Modified_Time\t\t\tCreated_Time\t\t   Attribute\n");
    for(i = 0; i < g_BootSector.u16RootEntryCount; i++)
    {
        flag = 1;
        if( (Root_Dir[i].FileName[0] == 0xE5) || (Root_Dir[i].FileName[0] == 0x00) )   /*Entry trong*/
        {
            flag = 0;
            i++;
        }
        if(flag == 1) /*thuc hien doc*/
        {
            if( (Root_Dir[i].u8Attributes ) != 0x0F)
            {
                for ( j = 0; j < 8; j++)
                {
                    printf("%c",Root_Dir[i].FileName[j]);
                }
                if(Root_Dir[i].u8Attributes != 0x10)
                {printf(".");}
                for ( j = 0; j < 3; j++)
                {
                    printf("%c",Root_Dir[i].Ext[j]);
                }
                if(Root_Dir[i].u8Attributes != 0x10)
                {
                    printf("   ");
                }
                else {
                    printf("    ");
                }
                if(Root_Dir[i].u8Attributes == 0x10)
                {
                    size = 0;
                    Into_Dir_To_CalSize(Root_Dir[i], &size, f);
                    printf("%-10d", size);
                    
                }
                else
                {
                    printf("%-10d", Root_Dir[i].u32Size);
                }
                Modf_Date(Root_Dir[i], &u16year, &u8mon, &u8day);
                Modf_Time(Root_Dir[i], &u8h, &u8m, &u8s);
                printf("%04d-%02d-%02d %02d:%02d:%02d",u16year, u8mon, u8day, u8h, u8m, u8s);

                Cre_Date(Root_Dir[i], &u16year, &u8mon, &u8day);
                Cre_Time(Root_Dir[i], &u8h, &u8m, &u8s);
                if(Root_Dir[i].u8Attributes == 0x10)
                {
                    printf("\t\t\t\t\t   ");
                    printf("D");
                    printf("\n");
                }
                else
                {
                    printf("\t\t");
                    printf("%04d-%02d-%02d %02d:%02d:%02d",u16year, u8mon, u8day, u8h, u8m, u8s);
                    printf("\n");
                }
            }
        }
    }
}
int STRCMP (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
    return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}
