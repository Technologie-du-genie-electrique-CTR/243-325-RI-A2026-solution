/**************************************************************************************************/
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
 **************************************************************************************************/

/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/

/* Standard header files */
#include <stdio.h>
#include <string.h>

/* Library header files */
#include "stm32f4xx_hal.h"

/* Application header files */
#include "application.h"
#include "main.h"
#include "standard_io.h"

/*==================================================================================================
 = TYPE DEFINITIONS
 =================================================================================================*/

/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/

#define PRINTF_DELAY 1000

/*==================================================================================================
 = GLOBAL VARIABLES
 =================================================================================================*/

static app_handle_t gh_app;

/*==================================================================================================
 = PRIVATE FUNCTION PROTOTYPES
 =================================================================================================*/

/*==================================================================================================
 = PUBLIC FUNCTIONS
 =================================================================================================*/

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
  gh_app->dummy = p_conf->dummy;
}


void app_process_loop (void)
{


  while(1)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
    HAL_Delay(PRINTF_DELAY);
  }
}

/*==================================================================================================
 = PRIVATE FUNCTIONS
 =================================================================================================*/

/*==================================================================================================
 = CALLBACKS
 =================================================================================================*/
