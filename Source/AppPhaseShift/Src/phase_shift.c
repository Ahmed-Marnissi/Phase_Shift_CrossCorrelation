/**
 * @file phase_shift.c
 * @brief Phase shift measurement using cross-correlation.
 *
 * This file implements the phase shift measurement between two signals using
 * CMSIS-DSP cross-correlation.
 *
 * @author Ahmed MARNISSI
 * @date March 20, 2025
 */

#include "phase_shift.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

/**
 * @brief Computes the cross-correlation and determines phase shift.
 * @param handle Pointer to the phase shift handle structure.
 */
static void computeCrossCorrelation(PhaseShift_Handle_t *handle);

/**
 * @brief Initializes the phase shift handle.
 *
 * This function initializes the phase shift structure, resets buffers, and
 * sets the sampling frequency.
 *
 * @param handle Pointer to the phase shift handle structure.
 * @param samplingFreq The sampling frequency in Hz.
 */
void phaseShift_Init(PhaseShift_Handle_t *handle, float32_t freq , float32_t samplingFreq)
{
    handle->currentIndex = 0;
    handle->resultLength = SIGNAL_LENGTH * 2 - 1;
    handle->processCrossCorrelation = 0;
    handle->samplingFreq = samplingFreq; /**< Initialize sampling frequency */
    handle->freq=freq;
    memset(handle->input1, 0, sizeof(handle->input1));
    memset(handle->input2, 0, sizeof(handle->input2));
    memset(handle->result, 0, sizeof(handle->result));
}

/**
 * @brief Processes a new sample and updates the phase shift measurement.
 *
 * This function stores incoming samples, checks if enough samples are available,
 * and computes the phase shift using cross-correlation when a full buffer is ready.
 *
 * @param handle Pointer to the phase shift handle structure.
 * @param in1_sample Input sample from signal 1.
 * @param in2_sample Input sample from signal 2.
 * @return The computed phase shift in degrees.
 */
float32_t phaseShift_ProcessSample(PhaseShift_Handle_t *handle, float32_t in1_sample, float32_t in2_sample)
{
    handle->input1[handle->currentIndex] = in1_sample;
    handle->input2[handle->currentIndex] = in2_sample;

    handle->currentIndex++;
    if (handle->currentIndex >= SIGNAL_LENGTH)
    {
        handle->currentIndex = 0;
        handle->processCrossCorrelation = 1; /**< Flag to trigger cross-correlation processing */
    }

    if (handle->processCrossCorrelation)
    {
        handle->processCrossCorrelation = 0;
        computeCrossCorrelation(handle);
    }

    return handle->phase;
}

/**
 * @brief Computes cross-correlation and extracts phase shift.
 *
 * This function uses CMSIS-DSP's `arm_correlate_f32` to compute the cross-correlation
 * between two signals. It finds the peak correlation index, calculates the time delay,
 * and determines the phase shift.
 *
 * @param handle Pointer to the phase shift handle structure.
 */
static void computeCrossCorrelation(PhaseShift_Handle_t *handle)
{
    /**< Perform cross-correlation */
    // Cross-correlation: calculate the correlation between current and voltage
    arm_correlate_f32(handle->input1, SIGNAL_LENGTH, handle->input2, SIGNAL_LENGTH, handle->result);

    // Find the index of the maximum value in the cross-correlation result
    handle->maxIndex = 0;
    float32_t maxValue = handle->result[0];
    for (int32_t i = 1; i < handle->resultLength; i++)
    {
        if (handle->result[i] > maxValue)
        {
            maxValue = handle->result[i];
            handle->maxIndex = i;
        }
    }

    // Calculate the time delay based on the index of the maximum peak
    handle->timeDelay = (float32_t)(handle->maxIndex - (SIGNAL_LENGTH - 1)) / handle->samplingFreq;
    handle->phase =handle->timeDelay *  handle->freq * 360.0f;
}
