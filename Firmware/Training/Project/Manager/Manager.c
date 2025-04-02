/**
 * Path:    Project/Manager
 * File:    Manager.c
 * Author:  CeNTI
 */

/* ************************************************************************************ */
/* * Includes                                                                         * */
/* ************************************************************************************ */

#include <stdio.h>
#include "stm32f4xx.h"
#include "Manager.h"
#include "../UI/UI.h"

/* ************************************************************************************ */
/* * Private Functions                                                                * */
/* ************************************************************************************ */

/* Manager debug. 0: Disabled; 1: Enabled. */
#define MANAGER_DEBUG_LEVEL                 (1)

/* Number of LEDs. */
#define NUMBER_OF_LEDS                      (4)

/* Number of ticks between each LED to turn on/off. */
#define BUTTON_LED_TICKS_BETWEEN_ON         (1000)
#define BUTTON_LED_TICKS_BETWEEN_OFF        (500)

/* ************************************************************************************ */
/* * Global Variables                                                                 * */
/* ************************************************************************************ */

/* Manager state. */
static int g_manager_state    = 0;

/* Mode ButtonLED state. */
static int g_button_led_state = 0;

/* ************************************************************************************ */
/* * Macros                                                                           * */
/* ************************************************************************************ */

/* Manager debug. */
#if ((defined(MANAGER_DEBUG_LEVEL)  && (MANAGER_DEBUG_LEVEL > 0))    )
    #define MANAGER_DEBUG(...) printf("[D]: " __VA_ARGS__)
#else
    #define MANAGER_DEBUG(...)
#endif

/* ************************************************************************************ */
/* * Private Functions Prototypes                                                     * */
/* ************************************************************************************ */

/**
 * @brief   ButtonLED Finite State Machine (FSM).
 *
 * @details This function will turn on/off the LEDs based on the button state.
 *          The LEDs will turn on one by one (1000 ticks between) while the button is
 *          pressed.
 *          The LEDs will turn off one by one (500 ticks between) while the button is not
 *          pressed.
 *
 * @param   None.
 *
 * @return  None.
 */
static void ButtonLED_FSM(void);

/**
 * @brief   Temperature Finite State Machine (FSM).
 *
 * @details 
 *
 * @param   None.
 *
 * @return  None.
 */
static void Temperature_FSM(void);

/* ************************************************************************************ */
/* * Public Functions                                                                 * */
/* ************************************************************************************ */

void Manager_Initialize(void)
{
    MANAGER_DEBUG("Manager will be initialized...\n");

    UI_Initialize();

    g_manager_state     = 0;
    g_button_led_state  = 0;

    MANAGER_DEBUG("... Manager initialized.\n");
}

void Manager_Loop(void)
{
    enum {
        STATE_IDLE          =  0,
        STATE_BUTTON_LED    =  1,
        STATE_TEMPERATURE   =  2,
    };

    switch (g_manager_state)
    {
        case STATE_IDLE:
        {
            /* Check slider position. */
            if (UI_GetSliderState())
            {
                MANAGER_DEBUG("Going to Button LED mode.\n");
                g_manager_state = STATE_BUTTON_LED;
                g_button_led_state = 0;
            }
            else
            {
                MANAGER_DEBUG("Going to Temperature mode.\n");
                g_manager_state = STATE_TEMPERATURE;
            }

            break;
        }
        case STATE_BUTTON_LED:
        {
            /* If slider switched to temperature mode. */
            if (!(UI_GetSliderState()))
            {
                MANAGER_DEBUG("Going to Temperature mode.\n");
                g_manager_state    = STATE_TEMPERATURE;
                g_button_led_state = 0;
                break;
            }

            ButtonLED_FSM();
            break;
        }
        case STATE_TEMPERATURE:
        {
            /* If slider switched to button LED mode. */
            if (UI_GetSliderState())
            {
                MANAGER_DEBUG("Going to Button LED mode.\n");
                g_manager_state = STATE_BUTTON_LED;
                break;
            }

            Temperature_FSM();
            break;
        }
        default:
        {
            MANAGER_DEBUG("Warning: Invalid state %d.\n", g_manager_state);
            g_manager_state = STATE_IDLE;
            break;
        }
    }
}

/* ************************************************************************************ */
/* * Private Functions                                                                * */
/* ************************************************************************************ */

static void ButtonLED_FSM(void)
{
    enum {
        STATE_INIT      =  0,
        STATE_TURN_ON   =  1,
        STATE_TURN_OFF  =  2,
        STATE_MINIMUM   =  3,
        STATE_MAXIMUM   =  4,
    };

    static uint32_t initial_ticks = 0;
    static uint8_t  led_index     = 0;

    switch (g_button_led_state)
    {
        case STATE_INIT:
        {
            MANAGER_DEBUG("Button LED FSM initialization.\n");

            /* Turn off all LEDs. */
            for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
            {
                UI_SetLEDState(i, false);
            }

            g_button_led_state = STATE_MINIMUM;
            led_index          = 0;

            break;
        }
        case STATE_TURN_ON:
        {
            /* If button is released. */
            if (!(UI_GetButtonState()))
            {
                g_button_led_state = STATE_TURN_OFF;
                initial_ticks      = HAL_GetTick();
                break;
            }

            /* If enough time has passed. */
            if ((HAL_GetTick() - initial_ticks) >= BUTTON_LED_TICKS_BETWEEN_ON)
            {
                if (led_index < NUMBER_OF_LEDS)
                {
                    MANAGER_DEBUG("LED %d: On\n", led_index);

                    UI_SetLEDState(led_index, true);
                    led_index++;
                }
                else
                {
                    MANAGER_DEBUG("Reached maximum.\n");

                    g_button_led_state = STATE_MAXIMUM;
                    led_index          = NUMBER_OF_LEDS;
                }

                initial_ticks = HAL_GetTick();
            }
            break;
        }
        case STATE_TURN_OFF:
        {
            /* If button is pressed. */
            if (UI_GetButtonState())
            {
                g_button_led_state = STATE_TURN_ON;
                initial_ticks      = HAL_GetTick();
                break;
            }

            /* If enough time has passed. */
            if ((HAL_GetTick() - initial_ticks) >= BUTTON_LED_TICKS_BETWEEN_OFF)
            {
                if (led_index > 0)
                {
                    led_index--;

                    MANAGER_DEBUG("LED %d: Off\n", led_index);

                    UI_SetLEDState(led_index, false);
                }
                else
                {
                    MANAGER_DEBUG("Reached minimum.\n");

                    g_button_led_state = STATE_MINIMUM;
                    led_index          = 0;
                }

                initial_ticks = HAL_GetTick();
            }
            break;
        }
        case STATE_MINIMUM:
        {
            /* If button is pressed. */
            if (UI_GetButtonState())
            {
                g_button_led_state = STATE_TURN_ON;
                initial_ticks      = HAL_GetTick();
                led_index          = 0;
                break;
            }

            break;
        }
        case STATE_MAXIMUM:
        {
            /* If button is released. */
            if (!(UI_GetButtonState()))
            {
                g_button_led_state = STATE_TURN_OFF;
                initial_ticks      = HAL_GetTick();
                break;
            }

            break;
        }
    }
}

static void Temperature_FSM(void)
{
    // TODO: ...
}


/* -- End of file -- */
