/*
 * AppFaultCheck.h
 *
 *  Created on: 2022. 5. 16.
 *      Author: MPC_Lab
 */

#ifndef APPFAULTCHECK_H_
#define APPFAULTCHECK_H_

#include "userparms.h"
#include "userdefines.h"

//===========================================================================
// Define
//===========================================================================

//===========================================================================
// Global Variable
//===========================================================================
extern volatile union      ERR_FLAGS       gstErrFlags;
extern Uint16              guiErrCode;
//===========================================================================
// External Function
//===========================================================================
void AppFC_Init(void);
void AppFC_CheckFault(void);
void AppFC_ClearFault(void);

#endif /* APPFAULTCHECK_H_ */


