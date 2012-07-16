/*
 * filtering_functions.h
 * Provides functions for the filtering of data
 * Author: Joshua James
 *
 */
 
#ifndef FILTERING_FUNCTIONS_H
#define FILTERING_FUNCTIONS_H

extern unsigned long filterSteering(unsigned long newValue);
extern void sampleSensors(void);
extern void calculateCanon(void);
extern void initFilters(void);
extern signed long mean_function(signed long array[], unsigned long num_values);

#endif
