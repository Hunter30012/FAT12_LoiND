#include "BootLib.h"
#include "RootLib.h"
#include "InOut.h"
#include "FAT_Table.h"

extern BootSector g_BootSector;
extern uint16_t g_DataStart_sector;

void Into_Dir_To_CalSize(RootDirectory Root_Dir, uint32_t *Size, FILE *f) /*Tinh size cua 1 Folder*/
{
    int j;
    int i;
    int clus;
    uint8_t flag;
    i = 0;
    uint16_t tempDir; /* vi tri cua sector dau tien*/
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    RootDirectory *Enry_Temp;
    /* doc 1 cluster theo so sector trong cluster */
    Enry_Temp = (RootDirectory *)malloc(g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector);
    /* doc 1 cluster theo so sector trong cluster */
    Read_Sectors(f, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
    for( i = 2; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++)
    {
        flag = 1;
        if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
        {
            flag = 0;
            i++;
        }
        if(flag == 1) /*thuc hien doc*/ 
        {
            if( ((Enry_Temp[i].u8Attributes ) != 0x0F) && (( Enry_Temp[i].u8Attributes  != 0x10 )) )
            {
                *Size = *Size + Enry_Temp[i].u32Size;
            }
            if(Enry_Temp[i].u8Attributes  == 0x10)
            {
                Into_Dir_To_CalSize(Enry_Temp[i], Size, f);
            }
        }
    }
    clus = Root_Dir.u16FirstClusterLow;
    while ( Find_Next_Cluster(clus, f) < 0xFF8 )
    {
        clus = Find_Next_Cluster(clus, f);
        tempDir = (clus - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
        Read_Sectors(f, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
        for( i = 0; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++)
        {
            flag = 1;
            if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
            {
                flag = 0;
                i++;
            }
            if(flag == 1) /*thuc hien doc*/ 
            {
                if( ((Enry_Temp[i].u8Attributes ) != 0x0F) && (( Enry_Temp[i].u8Attributes  != 0x10 )) )
                {
                    *Size = *Size + Enry_Temp[i].u32Size;
                }
                if(Enry_Temp[i].u8Attributes  == 0x10)
                {
                    Into_Dir_To_CalSize(Enry_Temp[i], Size, f);
                }
            }
        }
    }
    
    free(Enry_Temp);
}


void Display_Folder(RootDirectory *Root_Dir, RootDirectory Root,  FILE *f)
{
    uint32_t size;
    uint16_t clus;
    uint16_t tempDir;
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
    for( i = 2; i < ((g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32); i++)
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
                    printf("    ");}
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
    clus = Root.u16FirstClusterLow;
    while ( Find_Next_Cluster(clus, f) < 0xFF8 )
    {
        clus = Find_Next_Cluster(clus, f);
        tempDir = (clus - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
        Read_Sectors(f, tempDir, g_BootSector.u8SectorsPerCluster, Root_Dir);
        for( i = 0; i < ((g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32); i++)
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
                        printf("    ");}
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
}

void Into_Dir_To_Display(RootDirectory Root_Dir, FILE *f)  /* Root_Dir la 1 struct luu entry cua Thu muc */
{
    uint16_t tempDir; /* vi tri cua sector dau tien*/
    uint16_t clus;
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    RootDirectory *Enry_Temp;
    /*malloc 1 cluster*/
    Enry_Temp = (RootDirectory *)malloc(g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector);
    /* doc 1 cluster theo so sector trong cluster */
    Read_Sectors(f, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
    Display_Folder(Enry_Temp, Root_Dir ,f);
    free(Enry_Temp);
}

void PrintFile(RootDirectory Root_Dir, FILE *fp)
{
    char *s;
    uint16_t tempDir; /* vi tri cua sector dau tien*/
    int clus;
    int i;
    /*Vi tri cua Data*/
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    s = (char *)malloc( (g_BootSector.u8SectorsPerCluster) * (g_BootSector.u16BytesPerSector));
    Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, s);
    for( i = 0; i < g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector; i++)
    {
        printf("%c", s[i]);
    }
    clus = Root_Dir.u16FirstClusterLow;
    if( Root_Dir.u32Size != 0)
    {
        while ( Find_Next_Cluster(clus, fp) < 0xFF8 )
        {
            clus = Find_Next_Cluster(clus, fp);
            tempDir = (clus - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
            Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, s);
            for( i = 0; i < g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector; i++)
            {
                printf("%c", s[i]);
            }
        }
    }
    printf("\n");
}


/**************************************************************************/

void Read_FileRoot(RootDirectory *Root_Dir, FILE *fp)
{
    int k;
    int i;
    int j;
    uint8_t flag;
    unsigned char buffer_in[20];
    unsigned char str[20];
    printf("Nhap ten File can doc (Ten in hoa): ");
    fflush(stdin);
    scanf("%s", buffer_in);
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
                j = 0;
                while ((Root_Dir[i].FileName[j] != ' ') && (j < 8))
                {
                    str[j] = Root_Dir[i].FileName[j];
                    j++;
                }
                str[j] = '.';
                j++;
                k = 0;
                while ((Root_Dir[i].Ext[k] != ' ') && (k < 3))
                {
                    str[j] = Root_Dir[i].Ext[k];
                    j++;
                    k++;
                }
                str[j] = '\0';
                if(STRCMP(str, buffer_in) == 0)
                {
                    system("cls");
                    PrintFile(Root_Dir[i], fp);
                    printf("\n");
                    printf("Nhan phim bat ki de thoat ");
                    fflush(stdin);
                    getch();
                    system("cls");
                }    
            }
        }
    }
}

RootDirectory RootIntoFolder(RootDirectory *Root_Dir, FILE *fp, uint8_t *u8flag_Folder) /* Thu muc ME vao thu muc CON */
{   /* Tra ve Entry cua Thu muc can vao*/
    int i;
    int j;
    uint8_t flag;
    RootDirectory EntryFolder;
    unsigned char buffer_in[20];
    unsigned char str[20];
    printf("Nhap thu muc can vao trong (Ten in hoa): ");
    fflush(stdin);
    scanf("%s", buffer_in);
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
                j = 0;
                while ((Root_Dir[i].FileName[j] != ' ') && (j < 8))
                {
                    str[j] = Root_Dir[i].FileName[j];
                    j++;
                }
                str[j] = '\0';
                if(STRCMP(str, buffer_in) == 0)
                {
                    *u8flag_Folder = 1;
                    system("cls");
                    Into_Dir_To_Display(Root_Dir[i], fp);
                    EntryFolder = Root_Dir[i];
                    i = g_BootSector.u16RootEntryCount;
                    /*Root_Dir[i] mang Entry Foder muon truy cap */
                }
            }
        }
        
    }
    return EntryFolder;
}

/*********************************************/
/*Cho thu muc con ben trong*/
RootDirectory Return_Folder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root, uint8_t *flag)
{
    int i;
    int j;
    unsigned char buffer_in[20];
    unsigned char str[20];
    RootDirectory Entry_Folder;
    uint16_t tempDir; /* vi tri cua sector dau tien*/
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    RootDirectory *Enry_Temp;
    /*malloc 1 cluster*/
    Enry_Temp = (RootDirectory *)malloc(g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector);
    /* doc 1 cluster theo so sector trong cluster */
    Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
    /*Entry_Temp chua 1 danh sach cac Entry*/
    if(Enry_Temp[1].u16FirstClusterLow == 0x00)
    {
        system("cls");
        *flag = 0;
    }
    else
    {
        system("cls");
        Into_Dir_To_Display(Enry_Temp[1], fp);
        Entry_Folder = Enry_Temp[1];
    }
    free(Enry_Temp);
    return Entry_Folder;
}

void Read_FileFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root)
{
    int k;
    int i;
    int j;
    int clus;
    char c;
    uint8_t flag;
    unsigned char buffer_in[20];
    unsigned char str[20];

    uint16_t tempDir; /* vi tri cua sector dau tien*/
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    RootDirectory *Enry_Temp;
    /*malloc 1 cluster*/
    Enry_Temp = (RootDirectory *)malloc(g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector);
    /* doc 1 cluster theo so sector trong cluster */
    Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);

    printf("Nhap ten file can doc (Ten in hoa): ");
    fflush(stdin);
    scanf("%s", buffer_in);
    for( i = 2; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++) /*khac vong for*/
    {
        flag = 1;
        if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
        {
            flag = 0;
            i++;
        }
        if(flag == 1) /*thuc hien doc*/
        {
            if( (Enry_Temp[i].u8Attributes ) != 0x0F)
            {
                j = 0;
                while ((Enry_Temp[i].FileName[j] != ' ') && (j < 8))
                {
                    str[j] = Enry_Temp[i].FileName[j];
                    j++;
                }
                str[j] = '.';
                j++;
                k = 0;
                while ((Enry_Temp[i].Ext[k] != ' ') && (k < 3))
                {
                    str[j] = Enry_Temp[i].Ext[k];
                    j++;
                    k++;
                }
                str[j] = '\0';
                if(STRCMP(str, buffer_in) == 0)
                {
                    system("cls");
                    PrintFile(Enry_Temp[i], fp);
                    printf("\n");
                    printf("Nhan phim bat ki de thoat ");
                    fflush(stdin);
                    getch();
                    system("cls");
                    Into_Dir_To_Display(Root_Dir, fp);
                    i = ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 );
                }
            }
        }
    }
    clus = Root_Dir.u16FirstClusterLow;
    while ( Find_Next_Cluster(clus, fp) < 0xFF8 )
    {
        clus = Find_Next_Cluster(clus, fp);
        tempDir = (clus - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
        Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
        for( i = 0; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++)
        {
            flag = 1;
            if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
            {
                flag = 0;
                i++;
            }
            if(flag == 1) /*thuc hien doc*/
            {
                if( (Enry_Temp[i].u8Attributes ) != 0x0F)
                {
                    j = 0;
                    while ((Enry_Temp[i].FileName[j] != ' ') && (j < 8))
                    {
                        str[j] = Enry_Temp[i].FileName[j];
                        j++;
                    }
                    str[j] = '.';
                    j++;
                    k = 0;
                    while ((Enry_Temp[i].Ext[k] != ' ') && (k < 3))
                    {
                        str[j] = Enry_Temp[i].Ext[k];
                        j++;
                        k++;
                    }
                    str[j] = '\0';
                    if(STRCMP(str, buffer_in) == 0)
                    {
                        system("cls");
                        PrintFile(Enry_Temp[i], fp);
                        printf("\n");
                        printf("Nhan phim bat ki de thoat ");
                        fflush(stdin);
                        getch();
                        system("cls");
                        Into_Dir_To_Display(Root_Dir, fp);
                        i = ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 );
                    }
                }
            }
        }
    }
    free(Enry_Temp);
}

RootDirectory FolderIntoFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root) /*Thu muc Con vao thu muc con*/
{
    int i;
    int j;
    int clus;
    uint8_t flag;
    unsigned char buffer_in[20];
    unsigned char str[20];
    RootDirectory Entry_Folder;
    Entry_Folder = Root_Dir;
    uint16_t tempDir; /* vi tri cua sector dau tien*/
    tempDir = (Root_Dir.u16FirstClusterLow - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
    RootDirectory *Enry_Temp;
    /*malloc 1 cluster*/
    Enry_Temp = (RootDirectory *)malloc(g_BootSector.u8SectorsPerCluster * g_BootSector.u16BytesPerSector);
    /* doc 1 cluster theo so sector trong cluster */
    Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);

    printf("Nhap thu muc can vao trong (Ten in hoa): ");
    fflush(stdin);
    scanf("%s", buffer_in);
    for( i = 2; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++) /*khac vong for*/
    {
        flag = 1;
        if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
        {
            flag = 0;
            i++;
        }
        if(flag == 1) /*thuc hien doc*/ 
        {
            if( (Enry_Temp[i].u8Attributes ) != 0x0F)
            {
                j = 0;
                while ((Enry_Temp[i].FileName[j] != ' ') && (j < 8))
                {
                    str[j] = Enry_Temp[i].FileName[j];
                    j++;
                }
                str[j] = '\0';
                if(STRCMP(str, buffer_in) == 0)
                {
                    system("cls");
                    Into_Dir_To_Display(Enry_Temp[i], fp);
                    Entry_Folder = Enry_Temp[i];
                    i = (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32;
                }
            }
        }
    }
    clus = Root_Dir.u16FirstClusterLow;
    while ( Find_Next_Cluster(clus, fp) < 0xFF8 )
    {
        clus = Find_Next_Cluster(clus, fp);
        tempDir = (clus - 2) * g_BootSector.u8SectorsPerCluster + g_DataStart_sector;
        Read_Sectors(fp, tempDir, g_BootSector.u8SectorsPerCluster, Enry_Temp);
        for( i = 0; i < ( (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32 ); i++)
        {
            flag = 1;
            if( (Enry_Temp[i].FileName[0] == 0xE5) || (Enry_Temp[i].FileName[0] == 0x00) )   /*Entry trong*/
            {
                flag = 0;
                i++;
            }
            if(flag == 1) /*thuc hien doc*/ 
            {
                if( (Enry_Temp[i].u8Attributes ) != 0x0F)
                {
                    j = 0;
                    while ((Enry_Temp[i].FileName[j] != ' ') && (j < 8))
                    {
                        str[j] = Enry_Temp[i].FileName[j];
                        j++;
                    }
                    str[j] = '\0';
                    if(STRCMP(str, buffer_in) == 0)
                    {
                        system("cls");
                        Into_Dir_To_Display(Enry_Temp[i], fp);
                        Entry_Folder = Enry_Temp[i];
                        i = (g_BootSector.u16BytesPerSector * g_BootSector.u8SectorsPerCluster)/32;
                    }
                }
            }
        }
    }
    free(Enry_Temp);
    return Entry_Folder;
}
