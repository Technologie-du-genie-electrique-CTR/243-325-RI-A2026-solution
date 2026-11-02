/******************************************************************************/
/**
 * @file       application.c
 * @date       2026-09-04
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
 ******************************************************************************/

/*==============================================================================
 = INCLUDE FILES
 =============================================================================*/

/* Standard header files */
#include <stdio.h>
#include <string.h>

/* Library header files */
#include "stm32f4xx_hal.h"
#include "lcd_1602_driver.h"

/* Application header files */
#include "application.h"
#include "main.h"
#include "standard_io.h"

/*==============================================================================
 = DEFINITIONS
 =============================================================================*/

#define LOGIC_ANALYSER_PIN_OUTPUT(PIN_MEMBER, PIN_STATE) HAL_GPIO_WritePin(gh_app->PIN_MEMBER.port, gh_app->PIN_MEMBER.pin, PIN_STATE)

/*==============================================================================
 = TYPE DEFINITIONS
 =============================================================================*/

/*==============================================================================
 = GLOBAL VARIABLES
 =============================================================================*/

static app_handle_t gh_app;

/*==============================================================================
 = PRIVATE FUNCTION PROTOTYPES
 =============================================================================*/

void app_lcd_init (app_handle_t h);
void app_lcd_process (app_handle_t h);
void process_temperature_samples (app_handle_t h);
void display_console (app_handle_t h);
void display_lcd (app_handle_t h);

/*==============================================================================
 = PUBLIC FUNCTIONS
 =============================================================================*/

void app_init (app_config_t *p_conf)
{
  /* Définition locale statique de la structure du descripteur.
   * L'espace mémoire de la structure est allouée pour toute la durée du programme */
  static app_descriptor_t g_app_descriptor_object;

  /* Affection de l'adresse du descripteur vers le handle du descripteur (pointeur) */
  gh_app = (app_handle_t) &g_app_descriptor_object;

  /* Fonction qui affecte une valeur à chaque octet d'un espace mémoire
   * à partir d'une adresse d'origine et pour une quantité déterminée en octets */
  memset(gh_app, 0, sizeof(app_descriptor_t));

  /* Enregistrement des éléments de configuration dans la structure du descripteur */
  gh_app->la_a3       = p_conf->la_a3;
  gh_app->la_a4       = p_conf->la_a4;
  gh_app->la_a5       = p_conf->la_a5;
  gh_app->h_tim       = p_conf->h_tim;
  gh_app->h_adc       = p_conf->h_adc;
  gh_app->h_i2c       = p_conf->h_i2c;
}

void app_process_loop (void)
{
  printf("\033[2J\033[H");  // Clear terminal.
  fflush(stdout);           // Force printf to send data even if character "line feed" (\n) was not detected.
  printf("Projet 325\r\n");

  app_lcd_init(gh_app);

  HAL_ADC_Start_DMA(gh_app->h_adc, (uint32_t*)gh_app->temperature_sensor_sample, TEMPERATURE_SENSOR_SAMPLE_QTY);
  HAL_TIM_Base_Start(gh_app->h_tim);

  while(1)
  {
    if (gh_app->b_adc_evt_flag)
    {
      gh_app->b_adc_evt_flag = FALSE;

      process_temperature_samples(gh_app);
      display_lcd(gh_app);
      display_console(gh_app);
    }
  }
}

/*==============================================================================
 = PRIVATE FUNCTIONS
 =============================================================================*/

void app_lcd_init (app_handle_t h)
{
  init_lcd(h->h_i2c);
  clear_lcd();
}

void app_lcd_process (app_handle_t h)
{
  // Example test usage
  set_lcd_cursor(LCD_ROW_1, 0);
  send_lcd_string("Hello World");
  HAL_Delay(2000);
  clear_lcd();

  snprintf(h->lcd_row_table[LCD_ROW_1], LCD_ROW_CHAR_QTY, "TopRow: %d", LCD_ROW_1);
  snprintf(h->lcd_row_table[LCD_ROW_2], LCD_ROW_CHAR_QTY, "Bottom row: %d", LCD_ROW_2);

  set_lcd_cursor(LCD_ROW_1, 0);
  send_lcd_string(h->lcd_row_table[LCD_ROW_1]);
  set_lcd_cursor(LCD_ROW_2, 0);
  send_lcd_string(h->lcd_row_table[LCD_ROW_2]);
  HAL_Delay(2000);
  clear_lcd();
}

void process_temperature_samples (app_handle_t h)
{
  uint32_t average = 0;

  for(int i = 0; i < TEMPERATURE_SENSOR_SAMPLE_QTY; i++)
  {
    average += gh_app->temperature_sensor_sample[i];
  }

  h->temperature = average / 10;
}

void display_console (app_handle_t h)
{
  printf("\rTemperature ADC average: %04u", (unsigned int)h->temperature);
  fflush(stdout);
}

void display_lcd (app_handle_t h)
{
  snprintf(h->lcd_row_table[LCD_ROW_1], LCD_ROW_CHAR_QTY, "Temperature %04u", (unsigned int)h->temperature);

  set_lcd_cursor(LCD_ROW_1, 0);
  send_lcd_string(h->lcd_row_table[LCD_ROW_1]);
}

/*==============================================================================
 = CALLBACKS
 =============================================================================*/

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  HAL_GPIO_WritePin(LA_A3_GPIO_Port, LA_A3_Pin, GPIO_PIN_SET);
  if(hadc == gh_app->h_adc)
  {
    gh_app->b_adc_evt_flag = TRUE;
  }
  HAL_GPIO_WritePin(LA_A3_GPIO_Port, LA_A3_Pin, GPIO_PIN_RESET);
}
