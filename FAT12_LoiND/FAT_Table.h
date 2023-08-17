#ifndef _FATTABLE_H_
#define _FATTABLE_H_

/*******************************
Define
**********************************/
#define READ(index) ( ( (index)%2)  == 0 ) ? \
                    ( ( (fat[((index)/2)*3 + 1] & 0x0F) << 8 ) | fat[((index)/2) * 3] ) :\
                    ( ( fat[((index)/2)*3 + 1] >> 4 ) | ( fat[((index)/2)*3 + 2] << 4 ) )

/******************************************
uint16_t Find_Next_Cluster(uint16_t nowPois, FILE *fp);
@nowPois: index cua Cluster hien tai
@Tra ve index cua Cluster tiep theo
*******************************************/


uint16_t Find_Next_Cluster(uint16_t nowPois, FILE *fp);

#endif
