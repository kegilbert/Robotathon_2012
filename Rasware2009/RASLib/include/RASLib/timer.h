// ************************************
// TIMER AND WAIT FUNCTIONS AND DEFINES
// ************************************
#ifndef TIMER_H
#define TIMER_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

// Summary:	waits a specified amount of time
// Parameters:
//		ms: the number of milliseconds to wait
// Note:	Uses TIMER1. If you use Wait() or WaitUS(), you should not use TIMER1 elsewhere in your code;
//			If you use TIMER1 in your code, you should not use Wait() or WaitUS()
extern void Wait(unsigned long ms);

// Summary:	waits a specified amount of time
// Parameters:
//		ms: the number of microseconds to wait
// Note:	Uses TIMER1. If you use Wait() or WaitUS(), you should not use TIMER1 elsewhere in your code;
//			If you use TIMER1 in your code, you should not use Wait() or WaitUS()
extern void WaitUS(unsigned long us);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // TIMER_H
