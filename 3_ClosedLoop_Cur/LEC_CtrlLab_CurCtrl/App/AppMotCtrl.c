/*
 * AppMotCtrl.c
 *
 *  Created on: 2022. 4. 28.
 *      Author: MPC_Lab
 */
#include "F2837xD_device.h"
#include "AppMotCtrl.h"
#include "AppADCInput.h"
#include "AppPosSen.h"

#include "AppLab.h"

/*======================================================================
    Extern  Variable
======================================================================*/

/*===============================Output Variable=============================*/
CUR_TEST     gstCurOn;

/*===============================Speed Control Variable=============================*/
PI_CON      gstSpdCtrl_Tst;
float32     mfSpdCtrlOutLim;

/*===============================Openloop Variable=============================*/

float32      gfVs_vf;
/*============================Variation PWM Frequency=============================*/
VARI_FREQ   gstVariFreq;
/*======================================================================
    No Extern  Variable
======================================================================*/

/*======================================================================*/
void AppMotCtrl_Init(void)
{
    gstVariFreq.PWM_Carr_Freq   =   PWMCARRIER;
    gstVariFreq.PWM_Freq        =   PWM_FREQ;
    gstVariFreq.PWM_Tsmp        =   PWM_TSMP;
    gstVariFreq.Spd_Freq        =   SPD_FREQ;
    gstVariFreq.Spd_Tsmp            =   SPD_TSMP;
    gstVariFreq.Spd_Prescaler       =   SPD_PRESCALE;
    gstVariFreq.Tmax                =   TMAX_EPWM;
    gstVariFreq.ReqPWM_Freq         =   PWM_FREQ;
    gstVariFreq.ReqPWM_VariFreq_Amp    =  0.0;
    gstVariFreq.ReqPWM_VariFreq_Freq   =  0.0;


    gfVs_vf     = 1.0;

    mfSpdCtrlOutLim     = 20.0;




}

void AppMotCtrl_Init_PWMOff(void)
{
    gstCurCtrl.sum      = 0.0;
}

#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppMotCtrl_OpenLoop, "ramfuncs");
#endif
void AppMotCtrl_OpenLoop(void)
{
    //Synchronization
    gstCurCtrl.out = gfVs_vf;

}
#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppMotCtrl_DAxisTest, "ramfuncs");
#endif
void AppMotCtrl_DAxisTest(void)
{
    ;
}


