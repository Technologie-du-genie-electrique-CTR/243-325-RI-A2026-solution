/**************************************************************************************************/
/**
 * @file       application.h
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

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/

/* Standard header files */

/* Library header files */
#include "stm32f4xx_hal.h"
#include "custom_types.h"

/* Application header files */


/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/

/**
 * @brief
 * Structure de configuration de l'application.
 */
typedef struct app_config
{
  gpio_t led_blue;
  gpio_t button_s1;
  gpio_t relay;
} app_config_t;

/**
 * @brief
 * Structure du descripteur interne à l'application.
 */
typedef struct app_descriptor
{
  gpio_t led_blue;
  gpio_t button_s1;
  gpio_t relay;
} app_descriptor_t;

/**
 * @brief
 * Type pointeur de descripteur (handle) de l'application.
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
 * @param [in] p_conf
 * Pointer to configuration structure
 *
 * @return
 * none
 *
 **************************************************************************************************/
void app_init (app_config_t *p_conf);

/**************************************************************************************************/
/**
 * @brief
 * Application main processing function.
 *
 * @param [in,out]
 * none
 *
 * @return
 * none
 *
 **************************************************************************************************/
void app_process_loop (void);


#endif /* INC_APPLICATION_H_ */
