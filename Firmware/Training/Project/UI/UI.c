/**
 * Path:    Project/UI
 * File:    UI.h
 * Author:  CeNTI
 */

/* ********** Includes ********** */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_gpio.h"
#include "UI.h"


/* ********** Defines ********** */
#define UI_BUTTON_GROUP_AND_PIN     UI_BUTTON_GROUP, UI_BUTTON_PIN
#define UI_SLIDER_GROUP_AND_PIN     UI_SLIDER_GROUP, UI_SLIDER_PIN
#define UI_LED0_GROUP_AND_PIN       UI_LED0_GROUP, UI_LED0_PIN
#define UI_LED1_GROUP_AND_PIN       UI_LED1_GROUP, UI_LED1_PIN
#define UI_LED2_GROUP_AND_PIN       UI_LED2_GROUP, UI_LED2_PIN
#define UI_LED3_GROUP_AND_PIN       UI_LED3_GROUP, UI_LED3_PIN


/* ********** Global Variables ********** */
static bool g_button_pressed  = false;
static bool g_slider_position = false;

/* ********** ISR Functions ********** */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    if (gpio_pin == UI_BUTTON_PIN)
    {
        if (HAL_GPIO_ReadPin(UI_BUTTON_GROUP_AND_PIN) == GPIO_PIN_RESET)
        {
            g_button_pressed = true;
        }
        else
        {
            g_button_pressed = false;
        }
    }
    else if (gpio_pin == UI_SLIDER_PIN)
    {
        if (HAL_GPIO_ReadPin(UI_SLIDER_GROUP_AND_PIN) == GPIO_PIN_RESET)
        {
            g_slider_position = true;
        }
        else
        {
            g_slider_position = false;
        }
    }
}


/* ********** Functions ********** */
void UI_Initialize(void)
{
    g_button_pressed = (HAL_GPIO_ReadPin(UI_BUTTON_GROUP_AND_PIN) == GPIO_PIN_RESET) ? true : false;
    g_slider_position = (HAL_GPIO_ReadPin(UI_SLIDER_GROUP_AND_PIN) == GPIO_PIN_RESET) ? true : false;

    HAL_GPIO_WritePin(UI_LED0_GROUP_AND_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(UI_LED1_GROUP_AND_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(UI_LED2_GROUP_AND_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(UI_LED3_GROUP_AND_PIN, GPIO_PIN_SET);
}

bool UI_GetButtonState(void)
{
    return g_button_pressed;
}

bool UI_GetSliderState(void)
{
    return g_slider_position;
}

void UI_SetLEDState(uint8_t led, bool state)
{
    switch (led)
    {
        case 0:
            HAL_GPIO_WritePin(UI_LED0_GROUP_AND_PIN, (state) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(UI_LED1_GROUP_AND_PIN, (state) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(UI_LED2_GROUP_AND_PIN, (state) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(UI_LED3_GROUP_AND_PIN, (state) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        default:
            printf("Warning: Invalid LED number %d\n", led);
            break;
    }
}

/* -- End of file -- */
