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
void AppLab_Init(void)
{
    muiDacSel = 0;
    muiSpdCalCnt = 0;

    gfMecRad_old    = 0.0;
    gfDeltaRad      = 0.0;
    gfSpdRad        = 0.0;
    gfSpdRad_lpf    = 0.0;
    gfSpdRpm_lpf    = 0.0;

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
    gfTheta += gfFreq*_2PI*gfDeltaTime;
    BOUND_RAD(gfTheta)
    gfSig   = gfMag*__cos(gfTheta);
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

    gfMecRad = xfMecRad;
    gfDeltaRad   = xfMecRad - gfMecRad_old;

    BOUND_PI_RAD(gfDeltaRad)
    gfSpdRad = gfDeltaRad*1000.0;   //w(rad/s) result set

    //Calculate rotation speed (RPM, rev/min)
    gfSpdRpm  = gfSpdRad*OMEGA_TO_RPM;

    gfSpdRad_lpf = gfSpdRad_lpf*0.98 +  gfSpdRad*0.02; //Digital Filter
    gfSpdRpm_lpf = gfSpdRpm_lpf*0.98 +  gfSpdRpm*0.02; //Digital Filter

    gfMecRad_old = xfMecRad;
}

/*
 * AppLab_Get_DAC
 * Calling Frequency: 10kHz
 * Calling Location: AppSeq Interrupt (ISR)
 */
#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(AppLab_Get_DAC, "ramfuncs");
#endif
void AppLab_Get_DAC(void)
{
    if(0 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Gain   = 90.90909091;
        gstDAC_CH1.gfDAC_Value  = gfSig;
        gstDAC_CH1.gfDAC_Offset = 454.5454545;

        gstDAC_CH2.gfDAC_Gain   = _3MAX1_MIN_1;
        gstDAC_CH2.gfDAC_Value  = 10;
        gstDAC_CH2.gfDAC_Offset = 0.0;
    }
    else if(1 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Gain   = _3MAX1_MIN_0;
        gstDAC_CH1.gfDAC_Value  =  gfDCMotCurH_lpf;
        gstDAC_CH1.gfDAC_Offset =  0.0;

        gstDAC_CH2.gfDAC_Gain   = _3MAX10_MIN_0;
        gstDAC_CH2.gfDAC_Value  =  gfVs_vf;
        gstDAC_CH2.gfDAC_Offset =  0.0;
    }
    else if(2 == muiDacSel)
    {
        gstDAC_CH1.gfDAC_Value  = gfSpdRpm_lpf;
        gstDAC_CH1.gfDAC_Gain   = _3MAX500_MIN_0; //100rpm
        gstDAC_CH1.gfDAC_Offset = 0;


        gstDAC_CH2.gfDAC_Value   = gfMecRad;
        gstDAC_CH2.gfDAC_Gain    =_3MAX360_MIN_0;  //Rad 0~2Pi DAC 0V~3V
        gstDAC_CH2.gfDAC_Offset  = 0.0;
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
