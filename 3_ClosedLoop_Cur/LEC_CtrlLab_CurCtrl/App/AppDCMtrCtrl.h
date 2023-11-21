/*
 * AppDCMtrCtrl.h
 *
 *  Created on: Aug 24, 2023
 *      Author: JONGWON CHOI
 */

#ifndef APP_APPDCMTRCTRL_H_
#define APP_APPDCMTRCTRL_H_

//===========================================================================
// Define
//===========================================================================
typedef struct {
    float32     a_ph_out;
    float32     b_ph_out;
    float32     a_ph_out_lim;
    float32     b_ph_out_lim;
    float32     a_ph_out_err;
    float32     b_ph_out_err;
    float32     c_ph_out_err;
    float32     Ta;
    float32     Tb;
} FULL_BRI;

//===========================================================================
// Global Variable
//===========================================================================
//===========================================================================
// External Function
//===========================================================================
void AppDMCtrl_Init(void);
void AppDMCtrl_Run(float32 xfa_ph_out, float32 xfb_ph_out);
void AppDMCtrl_Off(void);

#endif /* APP_APPDCMTRCTRL_H_ */
