/*
 * application.h
 *
 *  Created on: Nov 14, 2025
 *      Author: simon
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_


/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/

/* Standard header files */

/* Library header files */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

/* Application header files */
#include "main.h"

/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/

#define ADC_DIGITAL_MAX 3500
#define ADC_DIGITAL_MIN 0
#define ADC_ANALOG_MAX  2800  //mV
#define ADC_ANALOG_MIN  0     //mV

#define VOLTAGE_STRING_SIZE 16+1

typedef enum bool
{
  FALSE = 0,
  TRUE
} bool_t;


/**
 * @brief
 * HAL internal descriptor of a UART instance.
 */
typedef struct app_descriptor
{
  I2C_HandleTypeDef   *h_i2c;   /**< I2C driver layer handle. */
  ADC_HandleTypeDef   *h_adc;
  TIM_HandleTypeDef   *h_tim;
  uint32_t            voltage;
  char                voltage_str[VOLTAGE_STRING_SIZE];
  volatile bool_t     b_voltage_flag;
} app_descriptor_t;


/**
 * @brief
 * HAL handle of a UART instance.
 */
typedef app_descriptor_t* app_handle_t;

/*==================================================================================================
 = GLOBAL VARIABLES
 =================================================================================================*/

/*==================================================================================================
 = PUBLIC FUNCTION PROTOTYPES
 =================================================================================================*/

/**************************************************************************************************/
/**
 * @brief
 * Application initialization function.
 *
 * @param [in,out] none
 *
 * @return
 * Application handle
 *
 **************************************************************************************************/
app_handle_t app_init (void);

/**************************************************************************************************/
/**
 * @brief
 * Application main processing function.
 *
 * @param [in,out] h
 * Application handle
 *
 * @return
 * none
 *
 **************************************************************************************************/
void app_process_loop (app_handle_t h);


#endif /* INC_APPLICATION_H_ */
