/*
 * AppLab.c
 *
 *  Created on: 2023. 8. 28.
 *      Author: MPC_Lab
 */

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2837xD_device.h"
#include "AppLab.h"
#include "F2837xD_dac.h"

#include <math.h>
#include "AppADCInput.h"
#include "AppFaultCheck.h"
#include "AppSeq.h"
#include "AppPosSen.h"
#include "AppMotCtrl.h"
#include "AppDCMtrCtrl.h"
/*======================================================================
    Extern  Variable
======================================================================*/
extern Uint16          muiSpdCalCnt;
/*======================================================================
    No Extern  Variable
======================================================================*/
Uint16 muiDacSel;

float32     gfMecRad;
float32     gfMecRad_old;
float32     gfDeltaRad;
float32     gfSpdRpm;
float32     gfSpdRad;
float32     gfSpdRad_lpf;
float32     gfSpdRpm_lpf;

float32    gfSig = 0.0;
float32    gfMag = 5.0;
float32    gfTheta = 0.0;
float32    gfFreq  = 10.0;
float32    gfDeltaTime  = 0.0001;

/*===============================Current Control Variable=============================*/
PI_CON      gstCurCtrl;
PI_CON      gstSpdCtrl;
float32     mfSpdCtrlOutLim;

void AppLab_Init(void)
{
    muiDacSel = 1;
    muiSpdCalCnt = 0;

    gfMecRad_old    = 0.0;
    gfDeltaRad      = 0.0;
    gfSpdRad        = 0.0;
    gfSpdRad_lpf    = 0.0;
    gfSpdRpm_lpf    = 0.0;

    gstCurCtrl.kp       = 0.0;
    gstCurCtrl.ki       = 0.0;
    gstCurCtrl.sum_lmt  = 9.0;
    gstCurCtrl.sum      = 0.0;

    gstCurOn.Cnt        = 0;
    gstCurOn.CurRef     = 0.5;
    gstCurOn.lim        = 500;

    gstSpdCtrl.kp          = 0.0;
    gstSpdCtrl.ki          = 0.0; //60^2/
    gstSpdCtrl.ka          = 0.0;
    gstSpdCtrl.sum         = 0.0;
    gstSpdCtrl.sum_lmt     = 10.0; //0.5A
    mfSpdCtrlOutLim        = 10.0;
}
/*
 * AppLab_Run_100usec
 * Calling Frequency: 10kHz
 * Calling Location: AppSeq Interrupt (ISR)
 */
#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppLab_Run_100usec, "ramfuncs");
#endif
void AppLab_Run_100usec(void)
{
    //Do not use endless loop and loop logic.
}

/*
 * AppLab_Cal_Mtr_Spd
 * Calling Frequency: 1kHz
 * Calling Location: AppPosSen.c Interrupt (ISR)
 * Input: xfMecRad (0 ~ 2*pi rad)
 */
#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppLab_Cal_Mtr_Spd, "ramfuncs");
#endif
void AppLab_Cal_Mtr_Spd(float xfMecRad)
{
    //Calculate rotation speed (rad/s)
}

/*
 * AppLab_Get_DAC
 * Calling Frequency: 10kHz
 * Calling Location: AppSeq Interrupt (ISR)
 */

#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppMotCtrl_Cur_H, "ramfuncs");
#endif
void AppMotCtrl_Cur_H(void)
{
    gstCurCtrl.ref = gstCurCtrl.set;
    gstCurCtrl.err = gstCurCtrl.ref - gfDCMotCurH;

    gstCurCtrl.sum += (gstCurCtrl.err*gstCurCtrl.ki)*gstVariFreq.PWM_Tsmp;

    if( gstCurCtrl.sum > gstCurCtrl.sum_lmt)              gstCurCtrl.sum = gstCurCtrl.sum_lmt;
    else if( gstCurCtrl.sum <= -gstCurCtrl.sum_lmt)       gstCurCtrl.sum = -gstCurCtrl.sum_lmt;

    gstCurCtrl.out = (gstCurCtrl.kp*gstCurCtrl.err + gstCurCtrl.sum); //- mstMotDynPara.Lq*gstMotCur.q_*gstTstPosSen.eOmega_LPF*gstCurCtrl.Decoeff;
}
/*
 * AppMotCtrl_Ctrl_Spd
 * Calling Frequency: 1kHz
 * Calling Location: AppSeq Interrupt (ISR)
 */

#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppMotCtrl_Ctrl_Spd, "ramfuncs");
#endif
void AppMotCtrl_Ctrl_Spd(void)
{
    gstSpdCtrl.ref = gstSpdCtrl.set*RPM_TO_OMEGA; //RPM to Omega

    gstSpdCtrl.err = gstSpdCtrl.ref - gstPosSen.SpdRad_lpf;

    gstSpdCtrl.sum +=(gstSpdCtrl.err*gstSpdCtrl.ki)*gstVariFreq.Spd_Tsmp;

    if(gstSpdCtrl.sum > gstSpdCtrl.sum_lmt)               gstSpdCtrl.sum =  gstSpdCtrl.sum_lmt;
    else if(gstSpdCtrl.sum <= -gstSpdCtrl.sum_lmt)    gstSpdCtrl.sum = -gstSpdCtrl.sum_lmt;

    gstSpdCtrl.out = (gstSpdCtrl.kp*gstSpdCtrl.err + gstSpdCtrl.sum);

    if(gstSpdCtrl.out > mfSpdCtrlOutLim)          gstSpdCtrl.out = mfSpdCtrlOutLim;
    else if(gstSpdCtrl.out < -mfSpdCtrlOutLim)    gstSpdCtrl.out = -mfSpdCtrlOutLim;


}
#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppLab_Get_DAC, "ramfuncs");
#endif
void AppLab_Get_DAC(void)
{
    if(0 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Gain   = 1;
        gstDAC_CH1.gfDAC_Value  = 3;
        gstDAC_CH1.gfDAC_Offset = _1_5DAC1_CMP;

        gstDAC_CH2.gfDAC_Gain   = _3MAX1_MIN_1;
        gstDAC_CH2.gfDAC_Value  = 10;
        gstDAC_CH2.gfDAC_Offset = _1_5DAC1_CMP;
    }
    else if(1 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Gain   = _3MAX180_MIN_0;
        gstDAC_CH1.gfDAC_Value  =  gstSpdCtrl.set;
        gstDAC_CH1.gfDAC_Offset =  0.0;

        gstDAC_CH2.gfDAC_Gain   = _3MAX180_MIN_0;
        gstDAC_CH2.gfDAC_Value  =  gstPosSen.SpdRpm_lpf;
        gstDAC_CH2.gfDAC_Offset =  0.0;
    }
    else if(2 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Value  = gfDCMotCurH;
        gstDAC_CH1.gfDAC_Gain   = _3MAX2_MIN_0;  //3V/2A 500mV:0.333A
        gstDAC_CH1.gfDAC_Offset = 0.0;

        gstDAC_CH2.gfDAC_Gain   = _3MAX180_MIN_0; //3V/180rpm 500mV:30rpm
        gstDAC_CH2.gfDAC_Value  =  gstPosSen.SpdRpm_lpf;
        gstDAC_CH2.gfDAC_Offset =  0.0;
    }
    else
    {
        ;
    }

    gfDAC_Val_1 = (gstDAC_CH1.gfDAC_Gain*gstDAC_CH1.gfDAC_Value) + gstDAC_CH1.gfDAC_Offset;
    gfDAC_Val_2 = (gstDAC_CH2.gfDAC_Gain*gstDAC_CH2.gfDAC_Value) + gstDAC_CH2.gfDAC_Offset;
    gfDAC_Val_3 = (gstDAC_CH3.gfDAC_Gain*gstDAC_CH3.gfDAC_Value) + gstDAC_CH3.gfDAC_Offset;
    gfDAC_Val_4 = (gstDAC_CH4.gfDAC_Gain*gstDAC_CH4.gfDAC_Value) + gstDAC_CH4.gfDAC_Offset;

    BOUND_VAL(gfDAC_Val_1 , DAC1_MAX_CMPA, DAC1_MIN_CMPA)
    BOUND_VAL(gfDAC_Val_2 , DAC1_MAX_CMPA, DAC1_MIN_CMPA)
    BOUND_VAL(gfDAC_Val_3 , DAC1_MAX_CMPB, DAC1_MIN_CMPB)
    BOUND_VAL(gfDAC_Val_4 , DAC1_MAX_CMPB, DAC1_MIN_CMPB)

    EPwm8Regs.CMPA.half.CMPA  = ((Uint16)gfDAC_Val_1);   // Set compare A value
    EPwm8Regs.CMPB.half.CMPB  = ((Uint16)gfDAC_Val_2);   // Set Compare B value
    EPwm7Regs.CMPA.half.CMPA  = ((Uint16)gfDAC_Val_3);   // Set compare A value
    EPwm7Regs.CMPB.half.CMPB  = ((Uint16)gfDAC_Val_4);   // Set Compare B value
}
