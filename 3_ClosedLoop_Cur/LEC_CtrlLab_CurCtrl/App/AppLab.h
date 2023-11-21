/*
 * AppLab.h
 *
 *  Created on: 2023. 8. 28.
 *      Author: MPC_Lab
 */

#ifndef APPLAB_H_
#define APPLAB_H_
#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================

//===========================================================================
// Global Variable
//===========================================================================
/*===============================Current Control Variable=============================*/
extern PI_CON      gstCurCtrl;
//===========================================================================
// External Function
//===========================================================================
void AppLab_Init(void);
void AppLab_Get_DAC(void);
void AppLab_Cal_Mtr_Spd(float xfMecRad);
void AppLab_Run_100usec(void);
void AppMotCtrl_Cur_H(void);
#endif /* APPLAB_H_ */
