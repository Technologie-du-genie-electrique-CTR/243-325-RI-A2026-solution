/**************************************************************************************************/
/**
 * @file       application.c
 * @date       2025-10-31
 * @author     Simon Boucher
 *
 * @brief
 * .
 *
 * @note
 *
 * @copyright
 * Copyright © Simon Boucher \n
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 * 2. Neither the name of the author or his organization may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * .
 * THIS SOFTWARE IS PROVIDED `AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE C2T3 BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************************************/

/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/

#include "application.h"

/* Standard header files */
#include "string.h" //memset
#include "stdio.h"  //snprintf

/* Library header files */
#include "lcd_1602_driver.h"

/* Application header files */

/*==================================================================================================
 = TYPE DEFINITIONS
 =================================================================================================*/

/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/

/*==================================================================================================
 = GLOBAL VARIABLES
 =================================================================================================*/

static app_handle_t gh_app;

/*==================================================================================================
 = PRIVATE FUNCTION PROTOTYPES
 =================================================================================================*/

void app_lcd_init (app_handle_t h);
void app_lcd_process (app_handle_t h);
void app_adc_process (app_handle_t h);

/*==================================================================================================
 = PUBLIC FUNCTIONS
 =================================================================================================*/

app_handle_t app_init (void)
{
  static app_descriptor_t g_app_descriptor_object;

  gh_app = (app_handle_t) &g_app_descriptor_object;

  memset(gh_app, 0, sizeof(app_descriptor_t));

  return gh_app;
}


void app_process_loop (app_handle_t h)
{
  app_lcd_init(h);

  while(1)
  {
    if (h->b_voltage_flag)
    {
      h->b_voltage_flag = FALSE;
      app_adc_process(h);
      app_lcd_process(h);
    }
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
  }
}


/*==================================================================================================
 = PRIVATE FUNCTIONS
 =================================================================================================*/

void app_lcd_init (app_handle_t h)
{
  init_lcd(h->h_i2c);
  set_lcd_cursor(0, 0);
  send_lcd_string("Appuyez sur");
  set_lcd_cursor(1, 0);
  send_lcd_string("le bouton");
}

void app_lcd_process (app_handle_t h)
{
  set_lcd_cursor(0, 0);
  send_lcd_string("Voltage:        ");
  set_lcd_cursor(1, 0);
  snprintf(gh_app->voltage_str, VOLTAGE_STRING_SIZE, "%4u mV         ", (unsigned int) gh_app->voltage);
  send_lcd_string(gh_app->voltage_str);
}

void app_adc_process (app_handle_t h)
{
  uint32_t adc_val = HAL_ADC_GetValue(h->h_adc);
  h->voltage = adc_val * (ADC_ANALOG_MAX - ADC_ANALOG_MIN) / (ADC_DIGITAL_MAX - ADC_DIGITAL_MIN);
}

/*==================================================================================================
 = CALLBACKS
 =================================================================================================*/

void HAL_GPIO_EXTI_Callback (uint16_t pin_id)
{
  switch (pin_id)
  {
  case BUTTON_S4_Pin:
    HAL_TIM_Base_Start_IT(gh_app->h_tim);
  default:
  }
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *h_tim)
{
  if (h_tim->Instance == TIM2)
  {
    HAL_ADC_Start_IT(gh_app->h_adc);
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}


void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *h_adc)
{
  if (h_adc->Instance == ADC1)
  {
    gh_app->b_voltage_flag = TRUE;
  }
}
