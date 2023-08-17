#ifndef _INOUT_H_
#define _INOUT_H_

/*********************************************
void Into_Dir_To_CalSize(RootDirectory Root_Dir, uint32_t *Size, FILE *f);
@Tinh size cua 1 Folder
@Root_Dir: la Entry cua Folder can tinh Size
@ Size: kich thuoc cua thu muc

void Display_Folder(RootDirectory *Root_Dir,RootDirectory Root, FILE *f);
@In ra man hinh 1 Folder
@Root_Dir tro toi chuoi cac Entry trong Cluster dau cua Thu muc can in
@Root: Luu Entry cua Thu muc can in

void PrintFile(RootDirectory Root_Dir, FILE *fp);
@In ra noi dung cua 1 file
@Root_Dir: la Entry cua file can in

void Read_FileRoot(RootDirectory *Root_Dir, FILE *fp);
@Doc file cua thu muc goc
@Root_Dir: Mang luu cac Entry cua thu muc goc

RootDirectory RootIntoFolder(RootDirectory *Root_Dir, FILE *fp, uint8_t *u8flag_Folder);
@Tra ve Entry cua Thu muc can vao
@u8flag_Folder: tao co cho vong while
@Root_Dir: Mang luu cac Entry cua thu muc goc

RootDirectory Return_Folder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root, uint8_t *flag);
@Tra ve Entry cua Thu muc thoat den
@Root_Dir: Luu Entry cua thu muc dang thao tac

void Read_FileFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root);
@Doc File trong Folder
@Root_Dir: Luu Entry cua thu muc dang thao tac

RootDirectory FolderIntoFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root);
@Tu thu muc vao thu muc ben trong
@Root_Dir: Luu Entry cua thu muc dang thao tac
@ *Root la con tro tro toi cac Entry cua thu muc goc
@Tra ve Entry cua Folder can vao trong
**********************************************/
void Into_Dir_To_CalSize(RootDirectory Root_Dir, uint32_t *Size, FILE *f);
void Display_Folder(RootDirectory *Root_Dir,RootDirectory Root, FILE *f);
void Into_Dir_To_Display(RootDirectory Root_Dir,  FILE *f);
void PrintFile(RootDirectory Root_Dir, FILE *fp);
void Read_FileRoot(RootDirectory *Root_Dir, FILE *fp);
RootDirectory RootIntoFolder(RootDirectory *Root_Dir, FILE *fp, uint8_t *u8flag_Folder);
RootDirectory Return_Folder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root, uint8_t *flag);
void Read_FileFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root);
RootDirectory FolderIntoFolder(RootDirectory Root_Dir, FILE *fp, RootDirectory *Root);

#endif
