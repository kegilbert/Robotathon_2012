//*****************************************************************************
//
// init.h - Initialize headers for using the initialize functions
//
// Original author: Nicu Stiurca
// Date created: 8-5-2009
//
// Changelog:
// 08-05-2009: initial creation
//
//*****************************************************************************

#ifndef __INIT_H__
#define __INIT_H__

//*****************************************************************************
//
// IF building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

// This SATURATE macro can be used to make sure a value falls between a given
// minimum and maximum value. The value, minimum, and maximum can be passed in
// in any order
#define SATURATE(a,b,c) ((b)<(a)?(c)<(b)?(b):(a)<(c)?(a):(c):(c)<(a)?(a):(b)<(c)?(b):(c))

//*****************************************************************************
//
// Global variables
//
//*****************************************************************************
extern unsigned long g_ulTicksPerSecond;
extern unsigned long g_ulTicksPerUs;
extern unsigned long g_ulPWMTicksPerSecond;

//*****************************************************************************
//
// You can use these functions to ensure the MPU is initialized properly.
//
//*****************************************************************************

// Summary:	Provides protection against locking yourserlf out of being able
//		to reprogram the board
// Note:	Always call this first before doing ANYTHING
extern void LockoutProtection(void);
// Summary:	Initialize the MCU, including setting it to run from PLL,
//		enabling SysTick and interrupts.
extern void InitializeMCU(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __INIT_H__

