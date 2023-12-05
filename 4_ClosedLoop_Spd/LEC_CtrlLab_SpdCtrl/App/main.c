/* ==========================================================================
System Name:  	Lab Ctrl System
File Name:		Main.c
Description:	DC Motor Drive
Originator:		MPC Lab
Programmer 	: 	Jongwon Choi
Check		:
================================================================================
 History:
--------------------------------------------------------------------------------
07-30-2021	Version 0.1   Modified
================================================================================*/

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2837xD_Ipc_drivers.h"
#include "userdefines.h"
#include "userparms.h"
#include "userinitialize.h"

//App
#include "AppADCInput.h"
#include "AppFaultCheck.h"
#include "AppSeq.h"
#include "AppPosSen.h"
#include "AppMotCtrl.h"
#include "AppDCMtrCtrl.h"
#include "AppLab.h"


/*======================================================================
    User Define Function
======================================================================*/
/* Initial and Setting Function   */
void Initial_Variable(void);                //  Initialize

/* Task Handling Function   */
void FcnTask_1ms(void);
void FcnTask_2ms(void);
void FcnTask_10ms(void);
void FcnTask_100ms(void);
void FcnTask_1s(void);

/*======================================================================
	etc. Variable
======================================================================*/


void main(void)
{
	/*-----------------------------------------------------------------------------
		Step 1
		Disable Global Interrupt & Interrupt Flag Clear
	-----------------------------------------------------------------------------*/

    DINT;
	IER = 0x0000;
	IFR = 0x0000;
	/*-----------------------------------------------------------------------------
	 	 Copy time critical code and Flash setup code to RAM
		This includes InitFlash(), Flash API functions and any functions that are
		assigned to ramfuncs section.
		The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
		symbols are created by the linker. Refer to the device .cmd file.
		=> Please, look at F2837xD_SysCtrl.c
	----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
		Step 2
		Initialize System Control: PLL, WatchDog, enable Peripheral Clocks
		This example function is found in the F28M3Xx_SysCtrl.c file.
	----------------------------------------------------------------------------*/
	InitSysCtrl();

	#ifdef _STANDALONE
		#ifdef _FLASH
		// Send boot command to allow the CPU2 application to begin execution
		IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
		#else
		// Send boot command to allow the CPU2 application to begin execution
		IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
		#endif
	#endif
	/*-----------------------------------------------------------------------------
		Step 3
		Initialize GPIO:
		This example function is found in the F28M3Xx_Gpio.c file and
		illustrates how to set the GPIO to it's default state.
	----------------------------------------------------------------------------*/
	InitGpio();
    InitEPwmGpio();

	/*-----------------------------------------------------------------------------
		Step 4
		Initialize PIE vector table: Pie Vector Table relocation
	-----------------------------------------------------------------------------*/
	InitPieCtrl();
	InitPieVectTable();

	/*-----------------------------------------------------------------------------
	    Step 5
	    5.1 Interrupt Service routine re-mapping and Interrupt vector enable
	 -----------------------------------------------------------------------------*/
	EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.ADCA1_INT  = &adca1_isr; //function for ADCA interrupt 1
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;


	EDIS;   // This is needed to disable write to EALLOW protected registers

	IER |= M_INT1; //Enable group 1 interrupts, ADCA1_INT, ADCB1_INT, ADCC1_INT
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; //TIMER 0 Interrupt


	/*-----------------------------------------------------------------------------
	  Step 6
	   6.1 Initialize Peripherals for User Application
	-----------------------------------------------------------------------------*/
	InitADC();
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 200MHz CPU Freq, 1 second Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 200, 1000); //1msec
    CpuTimer0Regs.TCR.all = 0x4000; //Interrupt enable, flag clear

    InitDACGpio(); //PWM DAC initialized
    InitEQep1Gpio(); //QEP initialized
	/*-----------------------------------------------------------------------------
		Step 7
		7.1 Initialize S/W modules and Variables
	-----------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------------
        Step 8
        Apps initialization
    -----------------------------------------------------------------------------*/
	AppMotCtrl_Init();
	AppAI_Init();
    AppFC_Init();
    AppPosSen_Init();
    AppSeq_Init();
    AppDMCtrl_Init();
    AppLab_Init();
	/*-----------------------------------------------------------------------------
	    Step 9
	    9.1 Enable Global realtime interrupt DBGM
	    9.2 Enable Global Interrupt
	-----------------------------------------------------------------------------*/
	ERTM;	/* Enable Global realtime interrupt DBGM */
	EINT;   /* Enable Global interrupt INTM */
	/*-----------------------------------------------------------------------------
	    Step 9
	    9.1 Idle Loop
	-----------------------------------------------------------------------------*/
    for(;;)
    {
      	if(gstTimeFlags._1ms)
        {
      	    FcnTask_1ms();
      		gstTimeFlags._1ms = CLEAR;
        }
    	if(gstTimeFlags._2ms)
    	{
    	    FcnTask_2ms();
    		gstTimeFlags._2ms = CLEAR;
    	}
    	if(gstTimeFlags._10ms)
    	{
    	    FcnTask_10ms();
    		gstTimeFlags._10ms = CLEAR;
    	}
    	if(gstTimeFlags._100ms)
    	{
    		FcnTask_100ms();
    		gstTimeFlags._100ms = CLEAR;
    	}
		if(gstTimeFlags._1s)
		{
		    FcnTask_1s();
			gstTimeFlags._1s = CLEAR;
		}
    }

}
/*======================================================================
	End of Main function.
======================================================================*/

void FcnTask_1ms(void)
{
;
}
void FcnTask_2ms(void)
{
;
}
void FcnTask_10ms(void)
{
;

}
void FcnTask_100ms(void)
{

    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = SET;

}
void FcnTask_1s(void)
{
    GpioDataRegs.GPATOGGLE.bit.GPIO31 = SET;
}


