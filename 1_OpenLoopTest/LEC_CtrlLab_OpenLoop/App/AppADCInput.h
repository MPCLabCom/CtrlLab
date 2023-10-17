/*
 * AppADCInput.h
 *
 *  Created on: 2022. 5. 10.
 *      Author: MPC_Lab
 */

#ifndef APPADCINPUT_H_
#define APPADCINPUT_H_

#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================

# define    ADC_V_DC             AdcdResultRegs.ADCRESULT0
# define    ADC_V_A              AdccResultRegs.ADCRESULT1
# define    ADC_V_B              AdcbResultRegs.ADCRESULT1
# define    ADC_V_C              AdcaResultRegs.ADCRESULT1

# define    ADC_V_REF            AdcaResultRegs.ADCRESULT2

# define    ADC_I_A              AdccResultRegs.ADCRESULT0
# define    ADC_I_B              AdcbResultRegs.ADCRESULT0
# define    ADC_I_C              AdcaResultRegs.ADCRESULT0

# define    ADC_V_IN             AdcaResultRegs.ADCRESULT0
# define    ADC_V_MC             AdcaResultRegs.ADCRESULT1  //Circuit Name
# define    ADC_I_L1             AdcaResultRegs.ADCRESULT2
# define    ADC_I_L5             AdcaResultRegs.ADCRESULT3
# define    ADC_T_IGBT1A         AdcaResultRegs.ADCRESULT4
# define    ADC_AIN_I1           AdcaResultRegs.ADCRESULT5

# define    ADC_E2               AdcbResultRegs.ADCRESULT0 //GND connect in Sensor board
# define    ADC_E5               AdcbResultRegs.ADCRESULT1 //GND connect in Sensor board
# define    ADC_I_L2             AdcbResultRegs.ADCRESULT2
# define    ADC_I_OUT            AdcbResultRegs.ADCRESULT3
# define    ADC_T_IGBT2A         AdcbResultRegs.ADCRESULT4
# define    ADC_AIN_I2           AdcbResultRegs.ADCRESULT5

# define    ADC_I_L3             AdccResultRegs.ADCRESULT2
# define    ADC_AIN_V1           AdccResultRegs.ADCRESULT3
# define    ADC_T_IGBT3A         AdccResultRegs.ADCRESULT4
# define    ADC_NTC_AUX2         AdccResultRegs.ADCRESULT5

# define    ADC_V_OUT            AdcdResultRegs.ADCRESULT0  //Circuit Name
# define    ADC_NTC_AUX1         AdcdResultRegs.ADCRESULT1
# define    ADC_I_L4             AdcdResultRegs.ADCRESULT2
# define    ADC_AIN_V2           AdcdResultRegs.ADCRESULT3
# define    ADC_T_IGBT4A         AdcdResultRegs.ADCRESULT4
# define    ADC_T_IGBT5A         AdcdResultRegs.ADCRESULT5 //No connect in Sensor board

/*==================Current Sensor Gain ==================*/
#define     A_PHASE_DYN_CUR_SENSOR_GAIN     (0.081)
#define     B_PHASE_DYN_CUR_SENSOR_GAIN     (0.081)
#define     C_PHASE_DYN_CUR_SENSOR_GAIN     (0.081)

#define     OFFSET_FAULT    1
#define     OFFSET_NORMAL   0

#define     MAX_ADC_OFFSET      2240.0
#define     MIN_ADC_OFFSET      2170.0

//===========================================================================
// Global Variable in AppADCInput
//===========================================================================

extern COORDINATE   gstMotCur;    // Real Mot Stator Current
extern float32      gfInvVdc;
extern float32      gfInvVdc_lpf;
extern THREE_PH     gstMotCurSign;
extern THREE_PH     gstMotCurAbs;

extern  float32     gfDCMotCurH;
extern  float32     gfDCMotCurH_lpf;
extern  float32     gfDCMotCurL;
extern  float32     gfDCMotCurL_lpf;
//===========================================================================
// External Function
//===========================================================================
void AppAI_Init(void);
void AppAI_CalOffset(void);
BOOL AppAI_SetOffset(void);
void AppAI_GetValue(void);



#endif /* APPADCINPUT_H_ */
