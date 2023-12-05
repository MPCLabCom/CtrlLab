/*
 * AppSeq.c
 *
 *  Created on: 2022. 5. 16.
 *      Author: MPC_Lab
 */

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2837xD_device.h"
#include "AppSeq.h"
#include  "F2837xD_dac.h"

//App
#include "AppADCInput.h"
#include "AppFaultCheck.h"
#include "AppPosSen.h"
#include "AppMotCtrl.h"
#include "AppDCMtrCtrl.h"
#include "AppLab.h"

volatile FLAGS      gstFlags        =   FLAGS_DEFAULTS;
TIME_FLAGS          gstTimeFlags      =   {0};

/* Interrupt Function */
interrupt void adca1_isr(void);
interrupt void cpu_timer0_isr(void);

/*======================================================================
    Speed Control
======================================================================*/
Uint16      ADC_Index = 0; //For calculation current sensor offset error
Uint32      count_int_adca=0;
Uint16      Start_PWM_CNT = 0;
Uint16      Finish_PWM_CNT = 0;
Uint16      muiVol_Cnt;
float32     gfSpdCmd_RPM;


void AppSeq_Init(void)
{
    gstFlags.CurSenCal          = SET;                //Dc offset
    gstFlags.PWM                = CLEAR;
    gstFlags.Const_Vc           = CLEAR;         //Filter Capacitor current Should be checked

    gstTimeFlags._100ms         = CLEAR;
    gstTimeFlags._10ms          = CLEAR;
    muiVol_Cnt= 0;

    gfSpdCmd_RPM = 0.0;
}



#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(adca1_isr, "ramfuncs");
#endif
interrupt void adca1_isr(void)
{
    //int_adca_PHASE=EPwm10Regs.TBCTR;
    //Start_PWM_CNT = EPwm1Regs.TBCTR; // For checking interrupt executed time
    count_int_adca++;

    if(gstFlags.CurSenCal) //ADC Input Check
    {
        AppAI_CalOffset();
        ADC_Index++;

        if(ADC_Index > 5000)//1sec
        {
            gstErrFlags.bit.SEN_ERR = AppAI_SetOffset();

            gstFlags.CurSenCal = 0;
            ADC_Index = 0;
        }
    }
    AppAI_GetValue();

    AppPosSen_GetVal();

    /*======================================================================
                Fault Detection Logic
    ======================================================================*/
    AppFC_CheckFault();
    // Operation Ready Check
    if(CLEAR == gstErrFlags.all)
    {
        gstFlags.Oper_Ready = SET ;
    }
    else
    {
        gstFlags.Oper_Ready = CLEAR ;
    }
    if(gstFlags.EMG_Stop)
    {
        gstFlags.PWM = CLEAR;
    }
    /*======================================================================
        Control Algorithm (Current control and Speed control)
    ======================================================================*/
    if(gstFlags.PWM)
    {
        PWM_EN  = SET;  // PWM Enable

        if(gstFlags.OLControl)
        {
            AppMotCtrl_OpenLoop();
        }
        else if(gstFlags.D_Axis_Test)
        {
            ;
        }
        else if(gstFlags.CurControl)
        {
            if(gstFlags.CurOn)
            {
                if(gstCurOn.Cnt>gstCurOn.lim)
                {
                    gstCurCtrl.set = 0.0;
                    gstCurOn.Cnt   = 0;
                    gstFlags.CurOn = CLEAR;
                }
                else
                {
                    gstCurOn.Cnt++;

                    gstCurCtrl.set = gstCurOn.CurRef;
                }
            }
            else if(gstFlags.SpdControl)
            {

                 if(!muiVol_Cnt)
                 {
                     gstSpdCtrl.set = 0.995*gstSpdCtrl.set + 0.005*gfSpdCmd_RPM;
                     AppMotCtrl_Ctrl_Spd();
                     muiVol_Cnt = gstVariFreq.Spd_Prescaler; //from 10 to 0
                     gstCurCtrl.set = gstSpdCtrl.out;
                 }
                 else
                 {
                     muiVol_Cnt--;
                 }
            }
            AppMotCtrl_Cur_H();
        }
        else if(gstFlags.SpdControl)
        {

            if(!muiVol_Cnt)
            {
                gstSpdCtrl.set = 0.995*gstSpdCtrl.set + 0.005*gfSpdCmd_RPM;
                AppMotCtrl_Ctrl_Spd();
                muiVol_Cnt = gstVariFreq.Spd_Prescaler; //from 10 to 0

                //gstCurCtrl.set = gstSpdCtrl.out;
                gstCurCtrl.out = gstSpdCtrl.out;
            }
            else
            {
                muiVol_Cnt--;
            }


        }
        else
        {

        }
    }
    else
    {
        PWM_DIS = SET;  //PWM EN Disable
        AppMotCtrl_Init_PWMOff();
        gfSpdCmd_RPM = 0;
        gstSpdCtrl.set = 0;
        gstSpdCtrl.sum=0;
        gstSpdCtrl.out = 0;
        /* Reset Controllers */
    }
    /*======================================================================
        Pulse width modulation
    ======================================================================*/
    AppDMCtrl_Run(gstCurCtrl.out,  -gstCurCtrl.out); //BiPolar
    /*======================================================================
        DAC Function Call and Check the Time Flags
    ======================================================================*/
    AppLab_Run_100usec();
    AppLab_Get_DAC();
    //Updata_DACValue();

    //Finish_PWM_CNT = EPwm1Regs.TBCTR; // For checking interrupt excuted time
    /*======================================================================
        ReinitIalize next ADC sequence
    ======================================================================*/
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

#ifdef __FLASH_PROGRAM
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
#endif
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;

   gstTimeFlags._1ms = SET;
   if((CpuTimer0.InterruptCount%2)==0){
      gstTimeFlags._2ms = SET;
      if((CpuTimer0.InterruptCount%10)==0){
          gstTimeFlags._10ms = SET;
          if((CpuTimer0.InterruptCount%100)==0){
              gstTimeFlags._100ms = SET;
              if((CpuTimer0.InterruptCount%1000)==0){
                  gstTimeFlags._1s = SET;
              }
          }
      }
   }

   //
   // Acknowledge this interrupt to receive more interrupts from group 1
   //
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


