/**
 * Path:    Project/UI
 * File:    UI.h
 * Author:  CeNTI
 */

/* ************************************************************************************ */
/* * Includes                                                                         * */
/* ************************************************************************************ */

#include "UI.h"

/* ************************************************************************************ */
/* * Global Variables                                                                 * */
/* ************************************************************************************ */

/* UI configuration. */
static st_UI_CONFIG g_config = {};

/* Button and slider states. */
static bool g_button_pressed  = false;
static bool g_slider_position = false;

/* ************************************************************************************ */
/* * Macros                                                                           * */
/* ************************************************************************************ */

/* Manager debug. */
#if ((defined(UI_DEBUG_LEVEL) && (UI_DEBUG_LEVEL > 0)))
    #define UI_DEBUG(...) printf("[D]: " __VA_ARGS__)
#else
    #define UI_DEBUG(...)
#endif

/* Delay. */
#define Delay_ms(_ms) g_config.Delay_ms(_ms)

/* ************************************************************************************ */
/* * ISR Functions                                                                    * */
/* ************************************************************************************ */

void UI_ISR_Button(void)
{
    #if (UI_INVERT_BUTTON == 1)
        g_button_pressed = (g_config.Button_GetPin() ? BUTTON_RELEASED : BUTTON_PRESSED);
    #else
        g_button_pressed = (g_config.Button_GetPin() ? BUTTON_PRESSED : BUTTON_RELEASED);
    #endif

    UI_DEBUG("Button %s\n", g_button_pressed ? "pressed" : "released");
}


void UI_ISR_Slider(void)
{
    #if (UI_INVERT_SLIDER == 1)
        g_slider_position = (g_config.Slider_GetPin() ? SLIDER_INACTIVE : SLIDER_ACTIVE);
    #else
        g_slider_position = (g_config.Slider_GetPin() ? SLIDER_ACTIVE : SLIDER_INACTIVE);
    #endif

    UI_DEBUG("Slider %s\n", g_slider_position ? "active" : "inactive");
}

/* ************************************************************************************ */
/* * Public Functions                                                                 * */
/* ************************************************************************************ */

void UI_Initialize(st_UI_CONFIG config)
{
    /* Save configuration. */
    g_config = config;

    /* Get initial state for button and slider. */
    g_button_pressed  = config.Button_GetPin();
    g_slider_position = config.Slider_GetPin();

    /* Initialize LEDs Off*/
    config.LED0_SetPin(LED_OFF);
    config.LED1_SetPin(LED_OFF);
    config.LED2_SetPin(LED_OFF);
    config.LED3_SetPin(LED_OFF);
}

bool UI_GetButtonState(void)
{
    return g_button_pressed;
}

bool UI_GetSliderState(void)
{
    return g_slider_position;
}

void UI_SetLEDState(uint8_t led,
                    bool    state)
{
    UI_DEBUG("LED %d %s\n", led, state == LED_ON ? "on" : "off");

    #if (UI_INVERT_LED == 1)
        state = (state == LED_ON ? false : true);
    #else
        state = (state == LED_ON ? true : false);
    #endif

    switch (led)
    {
        case 0:
            g_config.LED0_SetPin(state);
            break;
        case 1:
            g_config.LED1_SetPin(state);
            break;
        case 2:
            g_config.LED2_SetPin(state);
            break;
        case 3:
            g_config.LED3_SetPin(state);
            break;
        default:
            break;
    }
}

/* -- End of file -- */
