/**
 * Path:    Project/UI
 * File:    UI.h
 * Author:  CeNTI
 */

#ifndef __UI_H__
#define __UI_H__

/* ********** Includes ********** */
#include "stdbool.h"
#include "stdint.h"


/* ********** Defines ********** */
#define UI_BUTTON_GROUP             GPIOC
#define UI_BUTTON_PIN               GPIO_PIN_3
#define UI_SLIDER_GROUP             GPIOC
#define UI_SLIDER_PIN               GPIO_PIN_2
#define UI_LED0_GROUP               GPIOC
#define UI_LED0_PIN                 GPIO_PIN_7
#define UI_LED1_GROUP               GPIOC
#define UI_LED1_PIN                 GPIO_PIN_8
#define UI_LED2_GROUP               GPIOC
#define UI_LED2_PIN                 GPIO_PIN_9
#define UI_LED3_GROUP               GPIOA
#define UI_LED3_PIN                 GPIO_PIN_8


/* ********** Structures ********** */
typedef void (*t_GPIO_SetPin)(bool state);
typedef bool (*t_GPIO_GetPin)(void);

typedef struct {
    t_GPIO_SetPin   LED0_SetPin;
    t_GPIO_SetPin   LED1_SetPin;
    t_GPIO_SetPin   LED2_SetPin;
    t_GPIO_SetPin   LED3_SetPin;

    t_GPIO_GetPin   Button_GetPin;
    t_GPIO_GetPin   Slider_GetPin;
}st_UI_CONFIG;


/* ********** Functions Prototypes ********** */
/**
 * @brief Initialize the UI module.
 *
 * @param   None.
 *
 * @return  None.
 */
void UI_Initialize(void);

/* */
bool UI_GetButtonState(void);

/*  */
bool UI_GetSliderState(void);

/*  */
void UI_SetLEDState(uint8_t led, bool state);

void UI_ISR_Button(void);

void UI_ISR_Slider(void);

#endif /* __UI_H__ */

/* -- End of file -- */
