/**************************************************************************************************/
/**
 * @file       custom_types.h
 * @date       2026-09-10
 * @author     Simon Boucher
 *
 * @brief
 * .
 *
 * @note
 *
 **************************************************************************************************/

#ifndef INC_CUSTOM_TYPES_H_
#define INC_CUSTOM_TYPES_H_

/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/

/* Standard header files */

/* Library header files */

/* Application header files */
#include "stm32f4xx_hal.h"


/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/

/* Vérification que le type n'existe pas déjà */
#ifndef bool_t
/**
 * @brief
 * Définition d'un type booléen s'il n'existe pas déjà.
 */
typedef enum bool
{
  FALSE = 0,
  TRUE
} bool_t;
#endif

/**
 * @brief
 * Type d'état de DEL.
 */
typedef enum led_state
{
  LED_OFF = GPIO_PIN_RESET,
  LED_ON  = GPIO_PIN_SET
} led_state_t;

/**
 * @brief
 * Type d'état de bouton.
 */
typedef enum button_state
{
  BUTTON_PRESSED  = GPIO_PIN_RESET,
  BUTTON_RELEASED = GPIO_PIN_SET
} button_state_t;

/**
 * @brief
 * Type d'état de relais.
 */
typedef enum relay_state
{
  RELAY_OFF = GPIO_PIN_RESET,
  RELAY_ON  = GPIO_PIN_SET
} relay_state_t;

/**
 * @brief
 * Structure de broche E/S.
 */
typedef struct gpio_s
{
  GPIO_TypeDef *port;
  uint16_t pin;
  GPIO_PinState state;
} gpio_t;


/*==================================================================================================
 = GLOBAL VARIABLES
 =================================================================================================*/

/*==================================================================================================
 = PUBLIC FUNCTION PROTOTYPES
 =================================================================================================*/


#endif /* INC_CUSTOM_TYPES_H_ */
