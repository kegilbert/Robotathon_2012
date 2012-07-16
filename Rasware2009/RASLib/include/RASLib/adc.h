// *************************
// ADC FUNCTIONS AND DEFINES
// *************************
#ifndef ADC_H_
#define ADC_H_

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

// Summary: Initialize ADC Ports 0 through 3
extern void InitAdcPorts(void);

// Summary: Sample one of the ADC ports
//	port: which ADC port to sample. Must be one of 0, 1, 2, or 3.
//	returns: 10-bit unsigned value (0:1023) of voltage on the specified ADC port.
extern long sampleAdcPort(int port);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /*ADC_H_*/
