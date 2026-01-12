/**
 * @file phase_shift.h
 * @brief Header file for phase shift measurement using cross-correlation.
 *
 * This file defines the structures and functions for measuring phase shift
 * between two signals using cross-correlation with CMSIS-DSP.
 *
 * @author Ahmed MARNISSI
 * @date March 20, 2025
 */

#ifndef PHASE_SHIFT_H_
#define PHASE_SHIFT_H_

#include "stdint.h"
#include "arm_math.h"


/** @brief Defines the length of the signal buffer. */
#define SIGNAL_LENGTH 				1024U

/**
 * @brief Structure for phase shift computation.
 *
 * This structure holds the input signal buffers, computation results, and
 * other parameters needed for phase shift measurement.
 */
typedef struct
{
	float32_t freq ;
    float32_t input1[SIGNAL_LENGTH];    /**< Buffer for first input signal */
    float32_t input2[SIGNAL_LENGTH];    /**< Buffer for second input signal */
    float32_t result[SIGNAL_LENGTH * 2 - 1]; /**< Buffer for cross-correlation result */
    float32_t samplingFreq;             /**< Sampling frequency in Hz */
    uint32_t currentIndex;              /**< Current index for storing samples */
    uint32_t resultLength;              /**< Length of the cross-correlation result */
    int32_t maxIndex;                   /**< Index of the maximum correlation value */
    float32_t timeDelay;                /**< Calculated time delay in seconds */
    float32_t phase;                    /**< Calculated phase shift in degrees */
    uint8_t processCrossCorrelation;    /**< Flag to trigger cross-correlation processing */

} PhaseShift_Handle_t;

/**
 * @brief Initializes the phase shift measurement structure.
 *
 * This function initializes the phase shift handle, resets buffers,
 * and sets the sampling frequency.
 *
 * @param handle Pointer to the phase shift handle structure.
 * @param samplingFreq The sampling frequency in Hz.
 */
void phaseShift_Init(PhaseShift_Handle_t *handle, float32_t freq , float32_t samplingFreq);
/**
 * @brief Processes a new sample and updates the phase shift measurement.
 *
 * This function takes two input samples, stores them in buffers, and triggers
 * cross-correlation processing when enough samples are collected.
 *
 * @param handle Pointer to the phase shift handle structure.
 * @param in1_sample New sample from signal 1.
 * @param in2_sample New sample from signal 2.
 * @return The computed phase shift in degrees.
 */
float32_t phaseShift_ProcessSample(PhaseShift_Handle_t *handle, float32_t in1_sample, float32_t in2_sample);

#endif /* PHASE_SHIFT_H_ */
