/******************************************************************************

  Copyright (c) 2009 Copyright AgigA Tech, Inc. All right reserved! 

 ******************************************************************************
  File Name     : nvram_smbus_interface.h
  Version       : Initial Draft
  Author        : d
  Created       : 2011/9/2
  Last Modified :
  Description   : nvram_smbus_interface.c header file
  Function List :
 
 History Information Description                                               
 Date                         Author                         Modification      
 2011/9/2                     d                              Created function  

******************************************************************************/
#ifndef __NVRAM_SMBUS_INTERFACE_H__
#define __NVRAM_SMBUS_INTERFACE_H__

/*---------------------------------------------------------------------------*
 *                           MACRO  DECLARATIONS                             *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         STRUCTURE   DECLARATIONS                          *
 *---------------------------------------------------------------------------*/
typedef struct
{
    unsigned char dev_idx;
    unsigned char rw;
    unsigned char regNo;
    unsigned char data;
}NVS_SMB_CMD;

typedef struct
{
    unsigned char dev_idx;
    unsigned char rw;
    unsigned char regNo;
    unsigned char len;
    unsigned char data[64];
}NVS_SMB_BLK_CMD;



#define AGA_GET_U32(x) ( (*((unsigned char*)(x + 3)) << 24) | \
                            (*((unsigned char*)(x + 2)) << 16) | \
                                (*((unsigned char*)(x + 1)) << 8) | \
                                    (*((unsigned char*)(x + 0)) << 0) )

#define AGA_GET_U16(x) ( (*((unsigned char*)(x + 1)) << 8) | \
                            (*((unsigned char*)(x + 0)) << 0) )


#define AGA_GET_VER_STR(str,psrc,len) \
{ \
    memcpy(str,psrc,len); \
    str[len] = '\0';\
}

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

extern int nvdimm_backup_disable(unsigned char nvdimm_idx);
extern int nvdimm_backup_enable(unsigned char nvdimm_idx,unsigned char backup_type);
extern int nvdimm_cancel_backup(unsigned char nvdimm_idx);
extern int nvdimm_cancel_restore(unsigned char nvdimm_idx);
extern int nvdimm_do_factory_reset(unsigned char nvdimm_idx);
extern int nvdimm_get_backup_results(unsigned char nvdimm_idx,unsigned char* pValue);
extern int nvdimm_get_current_temperature(unsigned char nvdimm_idx, char* pValue);
extern int nvdimm_get_default_full_charge_voltage(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_fw_version_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_gtg_1(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_gtg_2(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_hw_version_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_last_backup_result(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_last_backup_time(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_last_charge_time(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_enable_cpm(unsigned char nvdimm_idx);
extern int nvdimm_get_last_gtg_1(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_last_gtg_2(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_last_restore_time(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_manufacture_data_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_manufacture_name_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_max_measure_charge_voltage(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_nfpool_percent(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_pcb_assembly_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_pcb_version_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_pgem_cap_stack_voltage(unsigned char nvdimm_idx,unsigned char* pValue);
extern int nvdimm_get_remain_voltage_lastbackup(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_restore_results(unsigned char nvdimm_idx,unsigned char* pValue);
extern int nvdimm_get_power_mode(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_sdram_density(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_serial_number_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int nvdimm_get_total_backup_cycle(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_total_power_cycle(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_ultracap_measure_interval(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_set_ultracap_measure_interval(unsigned char nvdimm_idx, unsigned int value);
extern int nvdimm_get_total_runtime(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_ultracap_capacitence_percent(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_uncorrectable_eccnums(unsigned char nvdimm_idx, unsigned int* pValue);
extern int nvdimm_get_upgrade_status(unsigned char nvdimm_idx, unsigned char* status);
extern int nvdimm_is_backup_done(unsigned char nvdimm_idx);
extern int nvdimm_is_eeprom_error(unsigned char nvdimm_idx);
extern int nvdimm_is_eeprom_ready(unsigned char nvdimm_idx);
extern int nvdimm_is_release_nd_image_done(unsigned char nvdimm_idx);
extern int nvdimm_is_restore_done(unsigned char nvdimm_idx);
extern int nvdimm_is_ultracap_measure_done(unsigned char nvdimm_idx);
extern int nvdimm_is_ultracap_measure_fault(unsigned char nvdimm_idx);
extern int nvdimm_light_switch_led(unsigned char nvdimm_idx,unsigned char on);
extern int nvdimm_read_eeprom(unsigned char nvdimm_idx,unsigned char eeprom_dev, unsigned char* pValue,unsigned short offset,unsigned short size);
extern int nvdimm_read_eeprom_data(unsigned char nvdimm_idx, unsigned char* data);
extern int nvdimm_release_nd_image(unsigned char nvdimm_idx);
extern int nvdimm_reset_system(unsigned char nvdimm_idx);
extern int nvdimm_set_eeprom_addr(unsigned char nvdimm_idx,unsigned short offset);
extern int nvdimm_set_eeprom_target(unsigned char nvdimm_idx, unsigned char target);
extern int nvdimm_start_backup(unsigned char nvdimm_idx);
extern int nvdimm_start_fw_download_header(unsigned char nvdimm_idx);
extern int nvdimm_start_restore(unsigned char nvdimm_idx);
extern int nvdimm_start_ultracap_measure(unsigned char nvdimm_idx, bool bclear_timer);
extern int nvdimm_write_eeprom(unsigned char nvdimm_idx,unsigned char eeprom_dev,unsigned char* pValue,unsigned short offset,  unsigned short size);
extern int nvdimm_write_eeprom_data(unsigned char nvdimm_idx, unsigned char data);
extern int nvdimm_write_upgrade_byte(unsigned char nvdimm_idx, unsigned char data);
extern int nvdimm_write_upgrade_word(unsigned char nvdimm_idx, unsigned char data0,unsigned char data1);
extern int pgem_get_capacitance_history(unsigned char nvdimm_idx, unsigned short* pValue);
extern int pgem_get_part_number_string(unsigned char nvdimm_idx, unsigned char* pValue);
extern int pgem_get_temperature_history(unsigned char nvdimm_idx, unsigned short* pValue);
extern int smbus_cmd_send(NVS_SMB_CMD *cmd);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __NVRAM_SMBUS_INTERFACE_H__ */
