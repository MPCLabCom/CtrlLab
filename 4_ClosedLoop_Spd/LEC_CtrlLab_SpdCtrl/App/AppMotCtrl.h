/*
 * AppMotCtrl.h
 *
 *  Created on: 2022. 4. 28.
 *      Author: MPC_Lab
 */

#ifndef APPMOTCTRL_H_
#define APPMOTCTRL_H_

#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================
typedef struct {
    float32     lim;
    float32     Cnt;
    float32     CurRef;
} CUR_TEST;

//===========================================================================
// Global Variable
//===========================================================================


/*===============================Output Variable=============================*/

/*===============================Speed Control Variable=============================*/
extern float32      gfOmega_vf;
extern float32      gftheta_vf;
extern float32      gfVs_vf;

/*======================================================================
    Variation PWM Frequency
======================================================================*/
extern VARI_FREQ    gstVariFreq;
extern CUR_TEST     gstCurOn;
//===========================================================================
// External Function
//===========================================================================
void AppMotCtrl_Init(void);
void AppMotCtrl_Init_PWMOff(void);
void AppMotCtrl_OpenLoop(void);


#endif /* APPMOTCTRL_H_ */
