/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "phase_shift.h"
/* USER CODE END Includes */

#define SIGNAL_FREQUENCY_HZ         10U
#define SAMPLE_FREQUENCY_HZ			1000U

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	float32_t current ;
	float32_t voltage ;

	float32_t simulatedphase ;
	float32_t measuredPhase ;
}cubeMonitorProbe_t ;


/**
 * @struct cubeMonitorProbe_t
 * @brief Structure for monitoring simulated and measured phase shifts
 */
cubeMonitorProbe_t monitorProbe;

/**
 * @struct PhaseShift_Handle_t
 * @brief Structure to handle phase shift calculations
 */
PhaseShift_Handle_t phaseShiftHandle;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


	/**
	     * @brief Initialize MCU
	     */
	HAL_Init();
  /**
     * @brief Initialize system clock
     */
  SystemClock_Config();

  /**
   * @brief Initialize phase shift computation with the sampling frequency
   */
  phaseShift_Init(&phaseShiftHandle, SIGNAL_FREQUENCY_HZ , SAMPLE_FREQUENCY_HZ);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  /*Sampling Routine */
	  static uint32_t previoust_time_ms ;
	  if( HAL_GetTick() - previoust_time_ms  >  ( 1 / SAMPLE_FREQUENCY_HZ ) * 1000U  )
	  {

		  static float32_t time;
		  time = time + 0.001   ;
		  float32_t time_shift  =   ( monitorProbe.simulatedphase / 360.0f) / SIGNAL_FREQUENCY_HZ ;
		  // Simulate a sine wave signal for current and voltage
		  float32_t currentSignal = 4.0f * sinf(2.0 * PI * SIGNAL_FREQUENCY_HZ* time);
		  float32_t voltageSignal =2.0f * sinf(2.0 * PI *  SIGNAL_FREQUENCY_HZ * (time + time_shift));
		  monitorProbe.current= currentSignal;
		  monitorProbe.voltage= voltageSignal;

		  monitorProbe.measuredPhase =phaseShift_ProcessSample ( &phaseShiftHandle , monitorProbe.current , monitorProbe.voltage);
		  previoust_time_ms = HAL_GetTick()  ;
	  }
	  else
	  {
		  /*Nothing to  do */
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

