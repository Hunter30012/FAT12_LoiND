#ifndef _ROOTLIB_
#define _ROOTLIB_

/**************************************************
Function:
void Read_Dir(FILE *f , RootDirectory **arr); 
@Doc cac thong tin thu muc goc va tra ve tai *arr

void Cre_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s);
@Doc thong tin thoi gian duoc tao
@Entry: Luu Entry cua file

void Cre_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day);
@Doc thong tin ngay thang duoc tao
@Entry: Luu Entry cua file

void Modf_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s);
@Doc thong tin thoi gian sua doi
@Entry: Luu Entry cua file hoac thu muc

void Modf_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day);
@Doc thong tin ngay thang sua doi
@Entry: Luu Entry cua file hoac thu muc

void Display(RootDirectory *Root_Dir, FILE *f);
@In ra thu muc goc
@ Root_Dir: Mang luu cac Entry trong thu muc goc

int STRCMP (const char *p1, const char *p2);
@So sanh 2 chuoi
***************************************************/
void Read_Dir(FILE *f , RootDirectory **arr);
void Cre_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s);
void Cre_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day);
void Modf_Time(RootDirectory Entry,uint8_t *h, uint8_t *m, uint8_t *s);
void Modf_Date(RootDirectory Entry,uint16_t *year, uint8_t *month, uint8_t *day);
void Display(RootDirectory *Root_Dir, FILE *f);
int STRCMP (const char *p1, const char *p2);

#endif
