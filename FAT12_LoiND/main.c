#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "BootLib.h"
#include "RootLib.h"
#include "InOut.h"
#include "FAT_Table.h"

BootSector g_BootSector;
uint16_t g_DataStart_sector;

int main ()
{
    uint16_t i;
    int j;
    uint8_t u8flag1;
    uint8_t u8flag_Folder;
    int Option_Root;
    int Option_Folder;
    RootDirectory *Root_Dir;
    RootDirectory Entry_Folder;
    FILE *fp = fopen("file3.IMA", "rb");

    Read_FirstSector(fp);
    Read_Dir(fp, &Root_Dir);
    
    u8flag1 = 1;
    u8flag_Folder = 0;
    while ( u8flag1 == 1 )
    {
    	system("cls");
    	u8flag1 = 1;
        Display(Root_Dir, fp);
        printf("Chon chuc nang: \n");
        printf("1. Doc file \n");
        printf("2. Vao thuc muc \n");
        printf("3. Thoat ra\n");
        printf(">>");
        scanf("%d", &Option_Root);
        switch ( Option_Root )
        {
        case 1:
            Read_FileRoot(Root_Dir, fp);
            break;
        case 2:
            Entry_Folder = RootIntoFolder(Root_Dir, fp, &u8flag_Folder);
            /* FLAG = 1  moi thuc hien */
            while( u8flag_Folder == 1)
            {
                printf("Chon chuc nang: \n");
                printf("1. Doc file \n");
                printf("2. Vao thuc muc \n");
                printf("3. Thoat ra\n");
                printf(">>");
                scanf("%d", &Option_Folder);
                switch ( Option_Folder )
                {
                case 1:
                    Read_FileFolder(Entry_Folder, fp, Root_Dir);
                    break;
                case 2:
                    Entry_Folder = FolderIntoFolder(Entry_Folder, fp, Root_Dir);
                    break;
                case 3:
                    Entry_Folder = Return_Folder(Entry_Folder, fp, Root_Dir, &u8flag_Folder);
                    break;
                default:
                    break;
                }
            }
            break;
        case 3:
            u8flag1 = 0;
            break;
        default:
            printf("Nhap lai: ");
            break;
        }
    }
    free(Root_Dir);
    fclose(fp);
    return 0;
}
