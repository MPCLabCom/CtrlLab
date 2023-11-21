/*
 * AppSeq.h
 *
 *  Created on: 2022. 5. 16.
 *      Author: MPC_Lab
 */

#ifndef APPSEQ_H_
#define APPSEQ_H_

#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================

//===========================================================================
// Global Variable
//===========================================================================
extern volatile FLAGS      gstFlags;
extern TIME_FLAGS          gstTimeFlags;
extern Uint16              guisel_CH;
//===========================================================================
// External Function
//===========================================================================
void AppSeq_Init(void);
interrupt void cpu_timer0_isr(void);
interrupt void adca1_isr(void);


#endif /* APPSEQ_H_ */
