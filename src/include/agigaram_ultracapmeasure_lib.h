/******************************************************************************

  Copyright (c) 2009 Copyright AgigA Tech, Inc. All right reserved! 

 ******************************************************************************
  File Name     : agigaram_ultracapmeasure_lib.h
  Version       : Initial Draft
  Author        : Max
  Created       : 2012/11/6
  Last Modified :
  Description   : agigaram_ultracapmeasure_lib.c header file
  Function List :
 
 History Information Description                                               
 Date                         Author                         Modification      
 2012/11/6                    Max                            Created function  

******************************************************************************/
#ifndef __AGIGARAM_ULTRACAPMEASURE_LIB_H__
#define __AGIGARAM_ULTRACAPMEASURE_LIB_H__

/*---------------------------------------------------------------------------*
 *                           MACRO  DECLARATIONS                             *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                         STRUCTURE   DECLARATIONS                          *
 *---------------------------------------------------------------------------*/
struct ultracap_measure{
    int  dimm_index;
    bool bclear_timer;
    int  capacitance_result;// in percent of %
    void* para;

    void (*measure_done_call_func)(void*,int);
};


typedef void (*FINISHE_FUNC)(void*,int);

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

extern int AgigaLib_StartCapMeasureThread(unsigned char nvdimm_idx,  bool bclear_timer, FINISHE_FUNC finish_call_func,void* para);
extern void AgigaLib_WaitCapMeasureThreadDone(unsigned char nvdimm_idx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AGIGARAM_ULTRACAPMEASURE_LIB_H__ */
