#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd.h"

#define PIN_BUTTON1             2u
#define PIN_BUTTON2             3u
#define PIN_BUTTON3             4u
#define PIN_BUZZER              12u

/* LCD Pin Connections */
#define PIN_LCD_RS              5u
#define PIN_LCD_RW              6u
#define PIN_LCD_EN              7u
#define PIN_LCD_D4              8u
#define PIN_LCD_D5              9u
#define PIN_LCD_D6              10u
#define PIN_LCD_D7              11u

#define BTN_PAD_VALUE_INC_TIME  4u
#define BTN_PAD_VALUE_DEC_TIME  2u
#define BTN_PAD_VALUE_ABRT      6u
#define BTN_PAD_VALUE_SP        1u

typedef enum
{
  NOT_PRESSED,
  BOUNCE,
  PRESSED
} button_state_t;

/* Signals of the Application */
typedef enum
{
  INC_TIME = 0,
  DEC_TIME,
  TIME_TICK,
  START_PAUSE,
  ABRT,
  /* Internal Activity Signals */
  ENTRY,
  EXIT
} protimer_signal_t;

/* Various States of the Application */
typedef enum
{
  IDLE = 0,
  TIME_SET,
  COUNTDOWN,
  PAUSE,
  STAT,
} protimer_state_t;

/* Generic (Super) Event Structure */
typedef struct 
{
  uint8_t sig;
} event_t;

/* For user generated events */
typedef struct 
{
  event_t super;
} protimer_user_event_t;

/* For Tick Events */
typedef struct 
{
  event_t super;
  uint8_t ss;
} protimer_tick_event_t;

typedef enum
{
  EVENT_HANDLED = 0,
  EVENT_IGNORED,
  EVENT_TRANSITION
} event_status_t;

/* Main Application Structure */
typedef struct 
{
  uint32_t          curr_time;
  uint32_t          elapsed_time;
  uint32_t          pro_time;
  protimer_state_t  active_state;
} protimer_t;


/* Public Function Prototpes */
void protimer_init( protimer_t *mobj );
/* Event Handlers Prototypes */
event_status_t IDLE_entry( protimer_t *const mobj, event_t const *const e );
event_status_t IDLE_exit( protimer_t *const mobj, event_t const *const e );
event_status_t IDLE_inc_time( protimer_t *const mobj, event_t const *const e );
event_status_t IDLE_pause( protimer_t *const mobj, event_t const *const e );
event_status_t IDLE_time_tick( protimer_t *const mobj, event_t const *const e );

event_status_t TIME_SET_entry( protimer_t *const mobj, event_t const *const e );
event_status_t TIME_SET_exit( protimer_t *const mobj, event_t const *const e );
event_status_t TIME_SET_inc_time( protimer_t *const mobj, event_t const *const e );
event_status_t TIME_SET_dec_time( protimer_t *const mobj, event_t const *const e );
event_status_t TIME_SET_abrt( protimer_t *const mobj, event_t const *const e );
event_status_t TIME_SET_start_pause( protimer_t *const mobj, event_t const *const e );

event_status_t COUNTDOWN_exit( protimer_t *const mobj, event_t const *const e );
event_status_t COUNTDOWN_time_tick( protimer_t *const mobj, event_t const *const e );
event_status_t COUNTDOWN_start_pause( protimer_t *const mobj, event_t const *const e );
event_status_t COUNTDOWN_abrt( protimer_t *const mobj, event_t const *const e );

event_status_t PAUSE_entry( protimer_t *const mobj, event_t const *const e );
event_status_t PAUSE_exit( protimer_t *const mobj, event_t const *const e );
event_status_t PAUSE_inc_time( protimer_t *const mobj, event_t const *const e );
event_status_t PAUSE_dec_time( protimer_t *const mobj, event_t const *const e );
event_status_t PAUSE_start_pause( protimer_t *const mobj, event_t const *const e );
event_status_t PAUSE_abrt( protimer_t *const mobj, event_t const *const e );

event_status_t STAT_inc_time( protimer_t *const mobj, event_t const *const e );
event_status_t STAT_exit( protimer_t *const mobj, event_t const *const e );
event_status_t STAT_time_tick( protimer_t *const mobj, event_t const *const e );

#endif 