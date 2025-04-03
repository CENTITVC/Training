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
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"
#include "i2c.h"

#include "Manager.h"

#include "../UI/UI.h"
#include "../Sensors/SHT31.h"

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

/* Mode Temperature state. */
static int g_temperature_state = 0;

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
/* * HAL Functions                                                                    * */
/* ************************************************************************************ */

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    if (gpio_pin == GPIO_PIN_3)
    {
        UI_ISR_Button();
    }
    else if (gpio_pin == GPIO_PIN_2)
    {
        UI_ISR_Slider();
    }
}

void LED0_SetPin(bool state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    }
}

void LED1_SetPin(bool state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    }
}

void LED2_SetPin(bool state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    }
}

void LED3_SetPin(bool state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    }
}

bool Button_GetState(void)
{
    return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_SET) ? BUTTON_RELEASED : BUTTON_PRESSED;
}

bool Slider_GetState(void)
{
    return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_SET) ? SLIDER_INACTIVE : SLIDER_ACTIVE;
}

void I2C_Write(uint8_t   i2c_address,
               uint8_t * tx_vec,
               uint8_t   tx_vec_size)
{
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(i2c_address << 1), &(tx_vec[0]),
                            tx_vec_size, 1000);
}

void I2C_Read(uint8_t   i2c_address,
              uint8_t * rx_vec,
              uint8_t   rx_vec_size)
{
    HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(i2c_address << 1), &(rx_vec[0]),
                           rx_vec_size, 1000);
}

uint8_t SHT31_CRC_Calculate(const uint8_t * data_vec,
                            uint8_t         data_vec_size)
{
    uint8_t crc = 0xFF;
    uint8_t polynomial = 0x31;

    for (size_t i = 0; i < data_vec_size; i++)
    {
        crc ^= data_vec[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc & 0x80) ? (crc << 1) ^ polynomial : (crc << 1);
        }
    }

    return crc;
}

void Delay_ms(uint16_t ms)
{
    HAL_Delay(ms);
}

/* ************************************************************************************ */
/* * Public Functions                                                                 * */
/* ************************************************************************************ */

void Manager_Initialize(void)
{
    st_UI_CONFIG ui_config = {
        .Button_GetPin  = Button_GetState,
        .Slider_GetPin  = Slider_GetState,
        .LED0_SetPin    = LED0_SetPin,
        .LED1_SetPin    = LED1_SetPin,
        .LED2_SetPin    = LED2_SetPin,
        .LED3_SetPin    = LED3_SetPin,
    };
    st_SHT31_CONFIG sht31_config = {
        .i2c_address    = SHT31_I2C_ADDRESS_GND,
        .I2C_Write      = I2C_Write,
        .I2C_Read       = I2C_Read,
        .CRC_Calculate  = SHT31_CRC_Calculate,
        .Delay_ms       = Delay_ms,
    };

    MANAGER_DEBUG("Manager: Initializing...\n");

    UI_Initialize(ui_config);
    SHT31_Initialize(sht31_config);

    g_manager_state     = 0;
    g_button_led_state  = 0;
    g_temperature_state = 0;

    MANAGER_DEBUG("Manager: ... Initialized.\n");
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
    enum {
        STATE_INIT          =  0,
        STATE_IDLE          =  1,
        STATE_GET_DATA      =  2,
        STATE_UPDATE_LEDS   =  3,
        STATE_DEFINE_BASE   =  4,
    };

    static float temp_base = 0;
    static float temp, hum = 0;
    static uint32_t last_get_tick = 0;

    if (UI_GetButtonState() == BUTTON_PRESSED)
    {
        g_temperature_state = STATE_DEFINE_BASE;
    }

    switch (g_temperature_state)
    {
        case STATE_INIT:
        {
            temp_base = 24.0f;

            g_temperature_state = STATE_GET_DATA;
            break;
        }
        case STATE_IDLE:
        {
            if (HAL_GetTick() - last_get_tick >= 500)
            {
                g_temperature_state = STATE_GET_DATA;
                break;
            }

            break;
        }
        case STATE_GET_DATA:
        {
            SHT31_GetData(&temp, &hum);

            MANAGER_DEBUG("Temperature mode: Temperature: %0.2f\n", temp);
            MANAGER_DEBUG("Temperature mode: Humidity: %0.2f\n", hum);

            last_get_tick = HAL_GetTick();

            g_temperature_state = STATE_UPDATE_LEDS;
        }
        case STATE_UPDATE_LEDS:
        {
            if (temp >= (temp_base - 1.0))  UI_SetLEDState(0, LED_ON);
            else                            UI_SetLEDState(0, LED_OFF);

            if (temp >= (temp_base + 0.5))  UI_SetLEDState(1, LED_ON);
            else                            UI_SetLEDState(1, LED_OFF);

            if (temp >= (temp_base + 1.0))  UI_SetLEDState(2, LED_ON);
            else                            UI_SetLEDState(2, LED_OFF);

            if (temp >= (temp_base + 1.5))  UI_SetLEDState(3, LED_ON);
            else                            UI_SetLEDState(3, LED_OFF);

            g_temperature_state = STATE_IDLE;
            break;
        }
        case STATE_DEFINE_BASE:
        {
            /* Wait for button to be released. */
            while (UI_GetButtonState() == BUTTON_PRESSED);

            MANAGER_DEBUG("Temperature mode: Redefining base temperature.\n");

            SHT31_GetData(&temp, &hum);

            MANAGER_DEBUG("Temperature mode: Temperature: %0.2f\n", temp);

            temp_base = temp;

            g_temperature_state = STATE_GET_DATA;
            break;
        }
    }
}


/* -- End of file -- */
