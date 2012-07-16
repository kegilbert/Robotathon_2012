// *****************************
// ENCODER FUNCTIONS AND DEFINES
// *****************************
#ifndef ENCODER_H
#define ENCODER_H

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

typedef enum { ENCODER_0, ENCODER_1 } encoder_t;	// encoder data type
typedef signed long encoder_count_t;				// encoder count type

// Summary:	Initializes the appropriate interrupts for encoders
// Note:	Always call this function before any other encoder-related functions
extern void InitializeEncoders(tBoolean invert0, tBoolean invert1);

// Summary:	Override the current encoder count(s) with the provided count(s)
extern void PresetEncoderCounts(encoder_count_t new_count0, encoder_count_t new_count1);
extern void PresetEncoderCount(encoder_t encoder, encoder_count_t new_count);

// Summary: Retrieve the current enconder count(s)
extern void GetEncoderCounts(encoder_count_t *pEncCount0, encoder_count_t *pEncCount1);
extern encoder_count_t GetEncoderCount(encoder_t encoder);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // ENCODER_H
