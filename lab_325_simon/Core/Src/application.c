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

void superloop_polling_blocking1 (void);
void superloop_polling_blocking2 (void);
void superloop_polling_nonblocking (void);
void superloop_interrupt_driven_nonblocking (void);
void event_driven_interrupt_driven_nonblocking1 (void);
void event_driven_interrupt_driven_nonblocking2 (void);
void event_driven_interrupt_driven_nonblocking2_lpm (void);
void event_driven_hw_event_driven_nonblocking (void);
void timer_process_evt (void);
void button_process_evt (void);

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
  gh_app->led_blue    = p_conf->led_blue;
  gh_app->button_s1   = p_conf->button_s1;
  gh_app->relay       = p_conf->relay;
  gh_app->la_a3       = p_conf->la_a3;
  gh_app->la_a4       = p_conf->la_a4;
  gh_app->la_a5       = p_conf->la_a5;
  gh_app->h_tim2      = p_conf->h_tim2;
}


void app_process_loop (void)
{
#ifdef SUPERLOOP_POLLING_BLOCKING1
  superloop_polling_blocking1();
#elifdef SUPERLOOP_POLLING_BLOCKING2
  superloop_polling_blocking2();
#elifdef SUPERLOOP_POLLING_NONBLOCKING
  superloop_polling_nonblocking();
#elifdef SUPERLOOP_INTERRUPT_DRIVEN_NONBLOCKING
  superloop_interrupt_driven_nonblocking();
#elifdef EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING1
  event_driven_interrupt_driven_nonblocking1();
#elifdef EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING2
  event_driven_interrupt_driven_nonblocking2();
#elifdef EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING2_LPM
  event_driven_interrupt_driven_nonblocking2_lpm();
#elifdef EVENT_DRIVEN_HW_EVENT_DRIVEN_NONBLOCKING
  event_driven_hw_event_driven_nonblocking();
#endif

  while(1)
  {
  }
}

/*==============================================================================
 = PRIVATE FUNCTIONS
 =============================================================================*/

void superloop_polling_blocking1 (void)
{
  const uint32_t c_toggle_delay = 1000; // milliseconds

  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  while (1)
  {
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);

    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_SET);

    // Toggle Blue LED
    HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);

    // Read S1 button state
    gh_app->button_s1.state = HAL_GPIO_ReadPin(gh_app->button_s1.port, gh_app->button_s1.pin);
    if (BUTTON_PRESSED == (button_state_t) gh_app->button_s1.state)
    {
      gh_app->relay.state = RELAY_ON;
    }
    else
    {
      gh_app->relay.state = RELAY_OFF;
    }
    HAL_GPIO_WritePin(gh_app->relay.port, gh_app->relay.pin, gh_app->relay.state);

    LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_RESET);
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);

    HAL_Delay(c_toggle_delay);
  }
}

void superloop_polling_blocking2 (void)
{
  const uint32_t c_toggle_delay = 1000; // milliseconds

  uint32_t tick_reference = 0;
  uint32_t tick_elapsed = 0;
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  while (1)
  {
    tick_reference = HAL_GetTick();

    LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_SET);

    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    // Toggle Blue LED
    HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);

    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);

    do
    {
      // Read S1 button state
      gh_app->button_s1.state = HAL_GPIO_ReadPin(gh_app->button_s1.port, gh_app->button_s1.pin);

      tick_elapsed = HAL_GetTick() - tick_reference;
      if (c_toggle_delay <= tick_elapsed)
      {
        LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_RESET);

        HAL_GPIO_WritePin(gh_app->relay.port, gh_app->relay.pin, RELAY_OFF);
      }
    }
    while (BUTTON_RELEASED == (button_state_t) gh_app->button_s1.state);

    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);

    HAL_GPIO_WritePin(gh_app->relay.port, gh_app->relay.pin, RELAY_ON);
  }
}

void superloop_polling_nonblocking (void)
{
  const uint32_t c_toggle_delay = 1000; // milliseconds

  uint32_t tick_reference = 0;
  uint32_t tick_elapsed = 0;
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  while (1)
  {
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);

    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    tick_elapsed = HAL_GetTick() - tick_reference;
    if (c_toggle_delay <= tick_elapsed)
    {
      // Toggle Blue LED
      HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);
      tick_reference = HAL_GetTick();
    }

    // Read S1 button state
    gh_app->button_s1.state = HAL_GPIO_ReadPin(gh_app->button_s1.port, gh_app->button_s1.pin);
    if (BUTTON_PRESSED == (button_state_t) gh_app->button_s1.state)
    {
      LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_SET);
      gh_app->relay.state = RELAY_ON;
    }
    else
    {
      LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_RESET);
      gh_app->relay.state = RELAY_OFF;
    }
    HAL_GPIO_WritePin(gh_app->relay.port, gh_app->relay.pin, gh_app->relay.state);

    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);
  }
}

void superloop_interrupt_driven_nonblocking (void)
{
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  HAL_TIM_Base_Start_IT(gh_app->h_tim2);

  while (1)
  {
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);
    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);
  }
}

void event_driven_interrupt_driven_nonblocking1 (void)
{
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  HAL_TIM_Base_Start_IT(gh_app->h_tim2);

  while (1)
  {
    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    if (gh_app->b_timer_flag)
    {
      gh_app->b_timer_flag = FALSE;

      // Toggle Blue LED
      HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);
    }

    if (gh_app->b_button_flag)
    {
      gh_app->b_button_flag = FALSE;

      // S1 button was pressed or released
      HAL_GPIO_TogglePin(gh_app->relay.port, gh_app->relay.pin);
    }
  }
}

void event_driven_interrupt_driven_nonblocking2 (void)
{
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  HAL_TIM_Base_Start_IT(gh_app->h_tim2);

  while (1)
  {
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);

    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    if (gh_app->b_timer_flag)
    {
      gh_app->b_timer_flag = FALSE;
      timer_process_evt();
    }

    if (gh_app->b_button_flag)
    {
      gh_app->b_button_flag = FALSE;
      button_process_evt();
    }

    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);
  }
}

void event_driven_interrupt_driven_nonblocking2_lpm (void)
{
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  HAL_TIM_Base_Start_IT(gh_app->h_tim2);

  while (1)
  {
    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_SET);

    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    if (gh_app->b_timer_flag)
    {
      gh_app->b_timer_flag = FALSE;
      timer_process_evt();
    }

    if (gh_app->b_button_flag)
    {
      gh_app->b_button_flag = FALSE;
      button_process_evt();
    }

    LOGIC_ANALYSER_PIN_OUTPUT(la_a3,GPIO_PIN_RESET);

    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
  }
}

void event_driven_hw_event_driven_nonblocking (void)
{
  uint32_t loop_ctr = 0;

  printf("\n\r");
  printf("--------------------------------------------------\n\r");
  printf("%s\n\r", __func__);
  printf("--------------------------------------------------\n\r");

  HAL_TIM_OC_Start_IT(gh_app->h_tim2, TIM_CHANNEL_1);

  while (1)
  {
    printf("Boucle #%u\n\r", (unsigned int)loop_ctr++);

    if (gh_app->b_button_flag)
    {
      gh_app->b_button_flag = FALSE;
      button_process_evt();
    }
  }
}

void timer_process_evt (void)
{
  // Toggle Blue LED
  HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);
}

void button_process_evt (void)
{
  // S1 button was pressed or released
  HAL_GPIO_TogglePin(gh_app->relay.port, gh_app->relay.pin);
}

/*==============================================================================
 = CALLBACKS
 =============================================================================*/

#if defined(SUPERLOOP_INTERRUPT_DRIVEN_NONBLOCKING)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == gh_app->button_s1.pin)
  {
    // S1 button was pressed or released
    HAL_GPIO_TogglePin(gh_app->relay.port, gh_app->relay.pin);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_SET);
  if (gh_app->h_tim2 == htim)
  {
    // Toggle Blue LED
    HAL_GPIO_TogglePin(gh_app->led_blue.port, gh_app->led_blue.pin);
  }
  LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_RESET);
}

#elif defined(EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING1) || defined(EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING2) || defined(EVENT_DRIVEN_INTERRUPT_DRIVEN_NONBLOCKING2_LPM)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == gh_app->button_s1.pin)
  {
    gh_app->b_button_flag = TRUE;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_SET);
  if (gh_app->h_tim2 == htim)
  {
    gh_app->b_timer_flag = TRUE;
  }
  LOGIC_ANALYSER_PIN_OUTPUT(la_a4,GPIO_PIN_RESET);
}

#elif defined(EVENT_DRIVEN_HW_EVENT_DRIVEN_NONBLOCKING)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == gh_app->button_s1.pin)
  {
    gh_app->b_button_flag = TRUE;
  }
}
#endif
