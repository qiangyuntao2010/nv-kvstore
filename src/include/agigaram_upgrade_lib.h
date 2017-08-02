/******************************************************************************

  Copyright (c) 2009 Copyright AgigA Tech, Inc. All right reserved! 

 ******************************************************************************
  File Name     : agigaram_upgrade_lib.h
  Version       : Initial Draft
  Author        : Max
  Created       : 2012/11/6
  Last Modified :
  Description   : agigaram_upgrade_lib.c header file
  Function List :
 
 History Information Description                                               
 Date                         Author                         Modification      
 2012/11/6                    Max                            Created function  

******************************************************************************/
#ifndef __AGIGARAM_UPGRADE_LIB_H__
#define __AGIGARAM_UPGRADE_LIB_H__

/*---------------------------------------------------------------------------*
 *                           MACRO  DECLARATIONS                             *
 *---------------------------------------------------------------------------*/
#define GET_U32(x) ( (*((unsigned char*)(x + 0)) << 24) | \
                        (*((unsigned char*)(x + 1)) << 16) | \
                                (*((unsigned char*)(x + 2)) << 8) \
                                    |(*((unsigned char*)(x + 3)) << 0) )
#define GET_U16(x) ( (*((unsigned char*)(x + 0)) << 8) | \
                        (*((unsigned char*)(x + 1)) << 0) )                                      
#define GET_STR(x) ( (unsigned char*)(x) )
#define LEN_PER_TRANS    32


#define HEADER_LEN          96
#define STR_LEN             8

#define UNI_IMG_ID_FW  0
#define UNI_IMG_ID_VPD 1
#define UNI_IMG_ID_SPD 2

#define LOG_STR_BUFFER 128



/*---------------------------------------------------------------------------*
 *                         STRUCTURE   DECLARATIONS                          *
 *---------------------------------------------------------------------------*/
typedef struct IMG_HEADER_S{
    unsigned int    fpga_sz;
    unsigned char   fpga_str[9];
    unsigned int    fw_sz;
    unsigned char   fw_str[9];
    unsigned int    pgem_fw_sz;
    unsigned char   pgem_fw_str[9];
}IMG_HEADER;

struct header_info{
  int       byte_count;
  UINT32    img_checksum;
  UINT32    img_start;
  UINT32    img_end;
};

typedef enum {
    UPGRADE_IGNORE = -1,
    UPGRADE_NORMAL = 0,
    UPGRADE_DONE = 1,
}upgrade_progress;


/*---------------------------------------------------------------------------*
 *                         VARIABLE   DECLARATIONS                           *
 *---------------------------------------------------------------------------*/
typedef void (*PROGRESS_FUNC)(void*,int,int);
/*---------------------------------------------------------------------------*
 *                          FUNCTION  DECLARATIONS                           *
 *---------------------------------------------------------------------------*/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
extern int AgigaLib_CheckLegacyUpgradeFile(unsigned char* file,unsigned int offset, IMG_HEADER* pImageInfo);
extern int AgigaLib_UpgradeFirmware(int *dimms_list, char *result,int *dimms_upgrade_status, unsigned int legacy, unsigned char* file, unsigned int off,unsigned int sz,PROGRESS_FUNC call_back ,void* para);
extern int AgigaLib_NvDimmOnlineUpgrade(int* dimms_list, char *result, int* dimms_upgrade_status, unsigned char* file, PROGRESS_FUNC call_back_func ,void* call_back_para);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AGIGARAM_UPGRADE_LIB_H__ */
