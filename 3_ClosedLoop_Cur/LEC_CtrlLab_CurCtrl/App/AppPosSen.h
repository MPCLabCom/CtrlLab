/*
 * AppPosSen.h
 *
 *  Created on: 2022. 5. 16.
 *      Author: MPC_Lab
 */

#ifndef APPPOSSEN_H_
#define APPPOSSEN_H_

#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================


typedef struct {
    float32         MecRad;
    float32         MecDeg;
    float32         MecRad_old;
    float32         DeltaRad;
    float32         Gain;
    float32         SpdRad;
    float32         SpdRpm;
    float32         SpdRad_lpf;
    float32         SpdRpm_lpf;
}DC_POS_SEN;

//===========================================================================
// Global Variable
//===========================================================================
extern DC_POS_SEN       gstPosSen;

//===========================================================================
// External Function
//===========================================================================
void AppPosSen_Init(void);
void AppPosSen_GetVal(void);
void AppSpdSen_RobotTest(void);
#endif /* APPPOSSEN_H_ */
