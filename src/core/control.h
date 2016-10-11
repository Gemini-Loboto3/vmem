/* ---------------------------------------------------------------------------
 * - (C) Sony Computer Entertainment. All Rights Reserved.
 * -
 * - Project:	Movie Player V2.0	
 * -
 * - Name:		control.h
 * -
 * - Author:	Vince Diesi
 * -
 * - Date:		13th Feb 1997
 * ---------------------------------------------------------------------------
 */

#ifndef __CONTROL_H
#define __CONTROL_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C"
{
#endif

#define USE_FULL_ANALOG		1

#include "ctrller.h"

/* ---------------------------------------------------------------------------
 * - MACRO DEFINITIONS
 * ---------------------------------------------------------------------------
 */

// Check controllers every second.
#define CHECK_CONTROLLERS		50

/* ---------------------------------------------------------------------------
 * - PUBLIC FUNCTION PROTOTYPES
 * ---------------------------------------------------------------------------
 */

void InitControllers(void);

/* ------------------------------------------------------------------------ */

//void StopControllers(void);

/* ------------------------------------------------------------------------ */

void CheckControllers(void);

/* ------------------------------------------------------------------------ */

u16 GetPadValue();

/* ------------------------------------------------------------------------ */

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif // __CONTROL_H 
