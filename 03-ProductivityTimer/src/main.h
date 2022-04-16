#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

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

#endif 