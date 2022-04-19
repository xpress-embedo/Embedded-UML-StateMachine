#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

/* Macros for buttons and buzzer pins */
#define PIN_BUTTON1                         2u    /* SET Button */
#define PIN_BUTTON2                         3u    /* OK Button */
#define PIN_BUZZER                          12u

#define BTN_PAD_VALUE_SET                   2u
#define BTN_PAD_VALUE_OK                    1u
#define BTN_PAD_VALUE_ABRT                  3u

#define MCU_SYS_CLOCK_FREQ                  16000000ul

#define TCCR1A_CTC_MODE                     (0U)
#define TCCR1B_CTC_MODE                     (1U << 3U)
#define TCCR1B_PRESCALER_1                  0x1UL
#define TCCR1B_PRESCALER_8                  0x2UL
#define TCCR1B_PRESCALER_64                 0x3UL
#define TCCR1B_PRESCALER_256                0x4UL

#define TIMER1_PRESCALER_1                  1UL
#define TIMER1_PRESCALER_8                  8UL
#define TIMER1_PRESCALER_64                 64UL
#define TIMER1_PRESCALER_256                256UL
#define TIMER1_PRESCALER_1024               1024UL

#define CONFIG_TCCR1B_PRESCALER             TCCR1B_PRESCALER_1
#define CONFIG_TIMER1_PRESCALER             TIMER1_PRESCALER_1
#define CONFIG_TICKS_PER_SECOND             1000UL

#define MS_PER_TICK                         (1000UL/CONFIG_TICKS_PER_SECOND)
#define TIMER1_OC_MATCH_VALUE               (((MCU_SYS_CLOCK_FREQ * MS_PER_TICK) / (CONFIG_TIMER1_PRESCALER * 1000UL))-1)

#define MS_TO_TICKS( timeInMs )             ( ( timeInMs  * CONFIG_TICKS_PER_SECOND ) / 1000UL )

extern uint8_t button_report_flag;

#endif