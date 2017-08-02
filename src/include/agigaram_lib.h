/******************************************************************************

  Copyright (c) 2009 Copyright AgigA Tech, Inc. All right reserved! 

 ******************************************************************************
  File Name     : agigaram_api.h
  Version       : Initial Draft
  Author        : Max
  Created       : 2011/11/3
  Last Modified :
  Description   : agigaram_api.c header file
  Function List :
 
 History Information Description                                               
 Date                         Author                         Modification      
 2011/11/3                    Max                            Created function  

******************************************************************************/
#ifndef __GIGARAM_API_H__
#define __GIGARAM_API_H__

/*---------------------------------------------------------------------------*
 *                           MACRO  DECLARATIONS                             *
 *---------------------------------------------------------------------------*/

typedef unsigned char BOOLEAN;
typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef unsigned char CHAR8;
typedef unsigned short CHAR16;

#define     FW_SPEC_MAJOR_MASK        0xFF00

#define     FW_SPEC_V070        0x0700
#define     FW_SPEC_V071        0x0701
#define     FW_SPEC_V072        0x0702
#define     FW_SPEC_V080        0x0800
#define     FW_SPEC_V081        0x0801


//Firmware SPEC define, this is mainly about EEPROM register
#define FW_SPEC_VERSION     FW_SPEC_V081


#define MAX_CORE          8
#define MAX_NODE          2 // Maximum CPU sockets supported
#define MAX_CH              4 // Maximum DIMM Channel supported
#define MAX_DIMM          3 // Max DIMM per channel

#define MAX_DIMMS_SUPPORT   MAX_NODE*MAX_CH*MAX_DIMM
#define MAX_MEM_REGION      8

#define SPD_MODULE_PART     18    // Number of bytes of module part                     

#define IOCTL_MAGIC                 'r'


#define IOCTL_SMBUS_BYTE_RW             _IO(IOCTL_MAGIC, 1)
#define IOCTL_SMBUS_CLTT_CONTROL        _IO(IOCTL_MAGIC, 2)
#define IOCTL_GET_NVDIMM_NUMS           _IO(IOCTL_MAGIC, 8)
#define IOCTL_GET_NVDIMM_SIZE           _IO(IOCTL_MAGIC, 14)
#define IOCTL_SYNC_D_CACHE              _IO(IOCTL_MAGIC, 15)
#define IOCTL_GET_NVDIMM_INFO           _IO(IOCTL_MAGIC, 16)
#define IOCTL_SPD_BYTE_RW               _IO(IOCTL_MAGIC, 17)

#define IOCTL_EEPROM_BYTE_RW            _IO(IOCTL_MAGIC, 20)



#define IOCTL_MAXNR                    64

#define SMB_BYTE_READ        0
#define SMB_BYTE_WRITE       1
#define SMB_WORD_READ        2
#define SMB_WORD_WRITE       3

enum {
    CLTT_DISABLE = 0x0,
    CLTT_ENABLE,
    CLTT_QUERY,
};

#define SPD_TYPE_DDR3       0x0B  // DDR3 SDRAM
#define SPD_TYPE_DDR4       0x0C  // DDR4 SDRAM

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BIT0
#define BIT0  1
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)
#define BIT8  (1 << 8)
#define BIT9  (1 << 9)
#endif
#ifndef BIT10
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)
#endif

//
// Global defines
//
#define  SUCCESS  0
#define  FAILURE  -1



/*---------------------------------------------------------------------------*
 *                         STRUCTURE   DECLARATIONS                          *
 *---------------------------------------------------------------------------*/
//
// Base structures
//
struct smbAddress {
  UINT16 rsvd:1;              // reserved bit
  UINT16 strapAddress:3;      // SMBus slave address (0...7)
  UINT16 deviceType:4;        // '0011' specifies TSOD, '1010' specifies EEPROM's, '0101' specifies other
  UINT16 busSegment:4;        // SMBus Segment
  UINT16 controller:4;        // JKT, ICH, etc...
};

struct smbDevice {
  UINT16            compId;   // SPD, MTS, DCP_ISL9072X, DCP_AD5247, DCP_UP6262, DCP_AD5337
  UINT16            imcId;     // Memory Controller ID
  UINT16			spdSize;	
  struct smbAddress address;  // SMBus Segment
};

struct nvdimm_device{
  int               sizeGB;
  int               status;

  /*the dimm slot # which this NVDIMM located*/
  int               node;
  int               ch;
  int               dimm;

  /*misc info for NVDIMM board*/
  int               ddrType;
  unsigned char     manufactureID[SPD_MODULE_PART+1];
  struct smbDevice  spd;        //NVDIMM Standard SPD Salve address
  struct smbDevice  nvCtl;      //NVDIMM Controller SMBus Salve address

  /*pointer to next device in chain*/
  struct nvdimm_device* next; 
};
struct nvdimm_info{
  struct    nvdimm_device dev;
  //struct    mem_region    memmap;
};

struct smb_command{
  UINT8 dev_index;   //index to locate the nvdimm device in this system, default 0
  UINT8 rw;   //Read or Write, 0 -- Read, 1 -- Write
  UINT8 reg;   //Register offset
  
  UINT8 data[4];  //Data
};




/*I2C Register Table*/
enum agigaram_register_table{

  EEPROM_BUSY  = 0x1,
  EEPROM_ADDRL = 0x2,
  EEPROM_ADDRH = 0x3,
  EEPROM_DATA  = 0x4,
  EEPROM_SEL   = 0x5,
  CAP_MEASURE      = 0x6,
  CAP_MEASURE_STAUS= 0x7,
  BACKUP_ENABLE = 0x8,
  BUSCHECK      = 0x9,
  BACKUP        = 0xA,
  RESTORE       = 0xB,
  RELEASE_NAND_IMG = 0xC,
  CPM_EN = 0xD,
  
  VCAP = 0xF,
  LEDS = 0x10,
  AUXEEADDR = 0x11,
  GTG_1 = 0x12,
  GTG_2 = 0x13,
  BACKUP_RESULT = 0x14,
  RESTORE_RESULT= 0x15,
  PWR_MD_STAT  = 0x16,
  DBDEST = 0x17,
  FACTORY_REST = 0x18,
  EEWREN = 0x19,
  RESET_SYSTEM = 0x1A,
  
  STDLD = 0x25,
  SDD = 0x26,
  DLDSTAT = 0x27,
  SDD_BLOCK = 0x28,

  REG_INVALID = 0xff
};

enum {
    BACKUP_DISABLE = 0x00,
    BACKUP_MODE_PGEM_PIN = 0x04,
    BACKUP_MODE_167PIN  = 0xEB,
    BACKUP_MODE_CKE_LOW = 0xBF,
};


#define DO_BACKUP  0x2E
#define CANCEL_BACKUP  0x0

#define DO_RESTORE  0x96
#define CANCEL_RESTORE  0x0

#define DO_RELEASE_NFIMG  0x37

#define DO_CAP_MEASURE   0x01


#define DO_FACTORY_RESET   0x92

#define DO_ENABLE_CPM	0xA5

#define DO_SYSTEM_RESET   0x45

#define PWR_MD_WATING 0xD1
#define PWR_MD_PGEM	  0x3C
#define PWR_MD_CPM    0xA5


#define EEPROM_NVDIMM 0x0
#define EEPROM_PGEM   0x1


#define EEPROM_MAX_BYTE    1024

//Coronado NVDIMM EEPROM Content
#if ((FW_SPEC_VERSION & FW_SPEC_MAJOR_MASK)== FW_SPEC_V070)

  #define USER          0x0000//-0x01FF //User Read/Write data
    #define USER_SIZE     512
  #define ENABLES           0x0200  //AutoRestore, SMB alert, Gen Call
  #define CAPMIVAL      0x0201  //Capacitance Measurement Interval
  //             0x0202-0x020F  Reserved
  #define TEMPHIST      0x0210//-0x022F //Temperature History
    #define TEMPHIST_SIZE 32
  #define CAPHIST           0x0230//-0x024F //Capacitance History
    #define CAPHIST_SIZE  32
  #define BAKRSLT1      0x0250  //Result of last BACKUP
  #define BAKRSLT2      0x0251  //Result of last BACKUP (contined)
  #define T_RUN         0x0252//-0x0255 //Run Time in Hours
    #define T_RUN_SIZE    4
  #define T_LASTPF      0x0256//-0x0259 //Run Time at Last Power Fail
    #define T_LASTPF_SIZE 4
  #define PWRCYCS           0x025A//-0x025B //Number of Power Cycles
    #define PWRCYCS_SIZE  2
  #define LASTTEMP      0x025C  //Last Temperature Measurement ¡ãC
  #define LASTCAP           0x025D  //Last Capacitance Measurement (%)
  #define LASTGTG1      0x025E  //Last GTG1
  #define LASTGTG2      0x025F  //Last GTG2
  #define T_LASTBU      0x0260//-0x0261 //Last Backup Time (sec)
    #define T_LASTBU_SIZE 2
  #define TOTBACKS      0x0262//-0x0263 //Total Backup Operations
    #define TOTBACKS_SIZE 2
  #define T_LASTCH      0x0264//-0x0265 //Last Charge Time
    #define T_LASTCH_SIZE 2
  #define V_REMAIN      0x0266//-0x0267 //Remaining Vcap after last Backup
    #define V_REMAIN_SIZE 2
  #define T_LASTRSTR        0x0268//-0x0269 //Last Restore Time
    #define T_LASTRSTR_SIZE 2
  #define UNCORNF           0x026A//-0x026B //Uncorrected NF errors, last RESTORE
    #define UNCORNF_SIZE  2
  #define NFPOOL            0x026C  //NF reserve pool (%)
  #define FWVER         0x026D//-0x0274 //CORONADO FW Version
    #define FWVER_SIZE    8
  #define HWVER         0x0275//-0x027C //CORONADO HW Version
    #define HWVER_SIZE    8
  #define CAPPN         0x027D//-0x0284 //Capacitor Part Number
    #define CAPPN_SIZE    8
  #define SN                0x0285//-0x028C //Serial Number
    #define SN_SIZE       8
  #define PCBVER            0x028D//-0x0294 //PCB Version
    #define PCBVER_SIZE       8
  #define MFDATE            0x0295//-0x029C //Manufacture Date
    #define MFDATE_SIZE       8
  #define ENDUSR            0x029D//-0x02A4 //Manufacturer Name (End user)
    #define ENDUSR_SIZE       8
  #define PCA               0x02A5//-0x02AC //PC Assembly Number
    #define PCA_SIZE       8
    
#else //if FW_SPEC_VERSION == FW_SPEC_V081

  #define USER          0x0000//-0x01FF //User Read/Write data
    #define USER_SIZE     512
  #define ENABLES           0x0200  //AutoRestore, SMB alert, Gen Call
  #define CAPMIVAL      0x0201  //Capacitance Measurement Interval
  //             0x0202-0x020F  Reserved
  #define BAKRSLT1      0x0250  //Result of last BACKUP
  #define BAKRSLT2      0x0251  //Result of last BACKUP (contined)
  #define T_RUN         0x0252//-0x0255 //Run Time in Hours
    #define T_RUN_SIZE    4
  
  #define PWRCYCS           0x025A//-0x025B //Number of Power Cycles
    #define PWRCYCS_SIZE  2
  #define LASTTEMP      0x025C  //Last Temperature Measurement ¡ãC
  #define LASTCAP           0x025D  //Last Capacitance Measurement (%)
  #define LASTGTG1      0x025E  //Last GTG1
  #define LASTGTG2      0x025F  //Last GTG2
  #define T_LASTBU      0x0260//-0x0261 //Last Backup Time (sec)
    #define T_LASTBU_SIZE 2
  #define TOTBACKS      0x0262//-0x0263 //Total Backup Operations
    #define TOTBACKS_SIZE 2
  #define T_LASTCH      0x0264//-0x0265 //Last Charge Time
    #define T_LASTCH_SIZE 2
  #define V_REMAIN      0x0266//-0x0267 //Remaining Vcap after last Backup
    #define V_REMAIN_SIZE 2
  #define T_LASTRSTR        0x0268//-0x0269 //Last Restore Time
    #define T_LASTRSTR_SIZE 2
  #define UNCORNF           0x026A//-0x026B //Uncorrected NF errors, last RESTORE
    #define UNCORNF_SIZE  2
  #define NFPOOL            0x026C  //NF reserve pool (%)
  #define FWVER         0x0280//-0x0274 //CORONADO FW Version
    #define FWVER_SIZE    5
  #define HWVER         0x0285//-0x027C //CORONADO HW Version
    #define HWVER_SIZE    2
  #define SN                0x0287//-0x028C //Serial Number
    #define SN_SIZE       8
  #define PCBVER            0x028F//-0x0294 //PCB Version
    #define PCBVER_SIZE       2
  #define MFDATE            0x0291//-0x029C //Manufacture Date
    #define MFDATE_SIZE       4
  #define ENDUSR            0x0295//-0x02A4 //Manufacturer Name (End user)
    #define ENDUSR_SIZE       2
  #define PCA               0x0297//-0x029C //PC Assembly Number
    #define PCA_SIZE          6
  #define DENSITY         0x029D//-0x029E   //NVDIMM Density (GB)
    #define DENSITY_SIZE      2
  #define CHARGEVOL         0x029F//-0x02A0 //Fully charge V of charger circuit (mV)
    #define CHARGEVOL_SIZE      2
  #define CHGMAXVOL         0x02A1//-0x02A2 //Highest voltage for cap measurement (mV)
    #define CHGMAXVOL_SIZE      2
    
  
  #define EEPROM_READONLY_START  (0x210)
  
  
  //PGEM EEPROM Contents, Coronado FW SPEC V0.8
  #define TEMPHIST          0x000//-0x020   //Temperature History
    #define TEMPHIST_SIZE    32
    
  #define CAPHIST           0x021//-0x040   //Capacitance History
    #define CAPHIST_SIZE    32
  
    
  #define CHARGER           0x041   //0=Charger on NVDIMM; 1=Charger on PGEM
  
  #define CAPACITANCE   0x042   //Spec CAP value of PGEM in Farads  Manufacturer
  #define PGEM_CHARGEVOL    0x043//-0x044   //Full chg V of chg circuit; 0xFFFF if no charger   Manufacturer
    #define PGEM_CHARGEVOL_SIZE    2
  #define PGEM_CHGMAXVOL    0x045//-0x046   //Highest voltage for cap measurement; 0xFFFF if no charger Manufacturer
    #define PGEM_CHGMAXVOL_SIZE    2
  #define POWERDET  0x047   //Power fail detection avail; 0 = no, 1 = yes   Manufacturer
  #define CHARGECUR 0x048//-0x049   //Cap charge current (mA); 0xFFFF if no charger Manufacturer
    #define CHARGECUR_SIZE    2
  #define PGEM_HWVER    0x04A//-0x04B   //PGEM HW Version   Manufacturer
    #define PGEM_HWVER_SIZE    2
  #define CAPPN 0x04C//-0x05D   //Capacitor Part Number Manufacturer
    #define CAPPN_SIZE    16
  #define PGEM_SN   0x05E//-0x63    //Product Serial Number Manufacturer
    #define PGEM_SN_SIZE    8
  #define PGEM_PCBVER   0x064//-0x065   //PCB Version   Manufacturer
    #define PGEM_PCBVER_SIZE    2
  #define PGEM_MFDATE   0x066//-0x069   //Manufacture Date (YYWW)   Manufacturer
    #define PGEM_MFDATE_SIZE    4
  #define PGEM_ENDUSR   0x06A//-0x06B   //Manufacturer Name (End user vendor code)  Manufacturer
    #define PGEM_ENDUSR_SIZE    2
  #define PGEM_PCA  0x06C//-0x076   //PC Assembly Number    Manufacturer
    #define PGEM_PCA_SIZE    11
  #define INITIALCAP    0x077   //Initial measured CAP value during Manuf   Manufacturer

#endif

/*---------------------------------------------------------------------------*
 *                         VARIABLE   DECLARATIONS                           *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          FUNCTION  DECLARATIONS                           *
 *---------------------------------------------------------------------------*/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern int AgigaLib_GetBackupMode(unsigned char nvdimm_idx);
extern int AgigaLib_GetCurrentTemperature(unsigned char nvdimm_idx);
extern int AgigaLib_GetNvDimmInfo(unsigned char nvdimm_idx,struct nvdimm_info* info);
extern unsigned long* AgigaLib_GetNvDimmMemAddress();
extern unsigned long AgigaLib_GetNvDimmMemSize();
extern int AgigaLib_NvDimmIoCtrl(unsigned int cmd, unsigned long* arg);
extern int AgigaLib_GetNvDimmNums();
extern int AgigaLib_GetTotalPowerCycles(unsigned char nvdimm_idx);
extern int AgigaLib_GetTotalRunTimeInHour(unsigned char nvdimm_idx);
extern int AgigaLib_GetUltraCapCurrentVoltage(unsigned char nvdimm_idx);
extern int AgigaLib_IsDDRBusReady(unsigned char nvdimm_idx);
extern int AgigaLib_IsLastRestoreSuccess(unsigned char nvdimm_idx);
extern int AgigaLib_IsNandFlashStatusOK(unsigned char nvdimm_idx);
extern int AgigaLib_IsNvdimmGoodToGo(unsigned char nvdimm_idx);
extern int AgigaLib_IsUltraCapConnected(unsigned char nvdimm_idx);
extern int AgigaLib_IsUltraCapFullCharged(unsigned char nvdimm_idx);
extern int AgigaLib_IsUltraCapHasEnoughEnergy(unsigned char nvdimm_idx);
extern int AgigaLib_IsValidImagePresent(unsigned char nvdimm_idx);
extern void AgigaLib_NvDimmDeInit();
extern int AgigaLib_NvDimmInit(bool bmap_memory);
extern int AgigaLib_ReadEEProm(unsigned char nvdimm_idx, int offset, int sizeRead, unsigned char* pData);
extern int AgigaLib_ReadNVRegister(unsigned char nvdimm_idx, unsigned char reg,unsigned char* data);
extern int AgigaLib_RleaseNandImage(unsigned char nvdimm_idx);
extern int AgigaLib_SetBackupMode(unsigned char nvdimm_idx,unsigned char mode);
extern bool AgigaLib_WordWriteNVRegister(unsigned char nvdimm_idx, unsigned char reg,unsigned char data0,unsigned char data1);
extern int AgigaLib_WriteEEProm(unsigned char nvdimm_idx, int offset, int sizeRead, unsigned char* pData);
extern bool AgigaLib_SetEEPromAllWritable(unsigned char nvdimm_idx, bool enable);
extern bool AgigaLib_WriteNVRegister(unsigned char nvdimm_idx, unsigned char reg,unsigned char data);
extern int AgigaLib_ReadSPD(unsigned char nvdimm_idx, UINT16 reg,unsigned char* data);
extern int AgigaLib_WriteSPD(unsigned char nvdimm_idx, UINT16 reg,unsigned char data);
extern int AgigaLib_ReadEEPromRegister(unsigned char nvdimm_idx, unsigned char eeprom_dev, unsigned short offset, unsigned char* data);
extern bool AgigaLib_WriteEEPromRegister(unsigned char nvdimm_idx, unsigned char eeprom_dev, unsigned short offset, unsigned char data);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __GIGARAM_API_H__ */
