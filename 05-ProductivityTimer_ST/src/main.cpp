#include "main.h"

/* Private Function Prototypes */
static void protimer_stable_table_init( protimer_t *const mobj );
static void protimer_event_dispatcher( protimer_t *const mobj, event_t const *const e );
static uint8_t process_button_pad_value( uint8_t btn_pad_value );
static void display_init( void );

/* Main Application Object */
static protimer_t protimer;

void setup() 
{
  /* This is used for debugging */
  Serial.begin(115200);
  pinMode( PIN_BUTTON1, INPUT );
  pinMode( PIN_BUTTON2, INPUT );
  pinMode( PIN_BUTTON3, INPUT );
  display_init();

  protimer_stable_table_init( &protimer );
  protimer_init( &protimer );
}

void loop() 
{
  uint8_t b1, b2, b3, btn_pad_value;
  protimer_user_event_t user_event;
  static protimer_tick_event_t tick_event;
  static uint32_t current_time = millis();

  /* Read the Buton Pad Status */
  b1 = digitalRead( PIN_BUTTON1 );
  b2 = digitalRead( PIN_BUTTON2 );
  b3 = digitalRead( PIN_BUTTON3 );

  btn_pad_value = (b1<<2u) | (b2<<1u) | (b3<<0u);
  /* Software Button debouncing */
  btn_pad_value = process_button_pad_value( btn_pad_value );
  if( btn_pad_value )
  {
    if( btn_pad_value == BTN_PAD_VALUE_INC_TIME )
    {
      user_event.super.sig = INC_TIME;
    }
    else if( btn_pad_value == BTN_PAD_VALUE_DEC_TIME )
    {
      user_event.super.sig = DEC_TIME;
    }
    else if( btn_pad_value == BTN_PAD_VALUE_SP )
    {
      user_event.super.sig = START_PAUSE;
    }
    else if( btn_pad_value == BTN_PAD_VALUE_ABRT )
    {
      user_event.super.sig = ABRT;
    }

    /* dispatch the event */
    protimer_event_dispatcher( &protimer, &user_event.super );
  }
  
  /* Dispatch the TIME TICK event every 100 ms */
  if( (millis() - current_time) >= 100 )
  {
    current_time = millis();
    tick_event.super.sig = TIME_TICK;
    tick_event.ss++;
    if( tick_event.ss > 10 )
    {
      tick_event.ss = 1;
    }
    /* dispatch the event */
    protimer_event_dispatcher( &protimer, &tick_event.super );
  }
}

/* Private Function Definitions */
static void protimer_event_dispatcher( protimer_t *const mobj, event_t const *const e )
{
  event_status_t status;
  protimer_state_t source, target;
  e_handler_t e_handler;
  
  source = mobj->active_state;
  /* This will be replaced as below status = protimer_state_machine( mobj, e ); */
  e_handler = (e_handler_t)(mobj->state_table[ (mobj->active_state*MAX_SIGNALS) + e->sig ] );
  /* Check for NULL Entries also */
  if( e_handler )
  {
    status = (*e_handler)( mobj, e );
  }

  if(status == EVENT_TRANSITION)
  {
    target = mobj->active_state;
    event_t ee;
    /* run the exit action for the source state */
    ee.sig = EXIT;
    /* mobj->active_state = source;
    protimer_state_machine( mobj, &ee ); */
    e_handler = (e_handler_t)(mobj->state_table[ (source*MAX_SIGNALS) + EXIT ] );
    if( e_handler )
    {
      (*e_handler)( mobj, &ee );
    }

    /* run the entry action for the target state */
    ee.sig = ENTRY;
    /* mobj->active_state = target;
    protimer_state_machine( mobj , &ee ); */
    e_handler = (e_handler_t)(mobj->state_table[ (target*MAX_SIGNALS) + ENTRY ] );
    if( e_handler )
    {
      (*e_handler)( mobj, &ee );
    }
  }
}

static uint8_t process_button_pad_value( uint8_t btn_pad_value )
{
  static button_state_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time;
  switch( btn_sm_state )
  {
    case NOT_PRESSED:
      if( btn_pad_value )
      {
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
    break;
    case BOUNCE:
      if( millis() - curr_time > 50 )
      {
        if( btn_pad_value )
        {
          btn_sm_state = PRESSED;
          return btn_pad_value;
        }
        else
        {
          btn_sm_state = NOT_PRESSED;
        }
      }
    break;
    case PRESSED:
      if( !btn_pad_value )
      {
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
    break;
  }
  return 0;
}

static void protimer_stable_table_init( protimer_t *const mobj )
{
  static e_handler_t protimer_state_table[MAX_STATES][MAX_SIGNALS] = 
  {
    /* SIGNALS-->   INC_TIME            DEC_TIME              TIME_TICK               START_PAUSE               ABORT               ENTRY               EXIT */
    [IDLE]      = { &IDLE_inc_time,     NULL,                 &IDLE_time_tick,        &IDLE_pause,              NULL,               &IDLE_entry,        &IDLE_exit },
    [TIME_SET]  = { &TIME_SET_inc_time, &TIME_SET_dec_time,   NULL,                   &TIME_SET_start_pause,    &TIME_SET_abrt,     &TIME_SET_entry,    &TIME_SET_exit},
    [COUNTDOWN] = { NULL,               NULL,                 &COUNTDOWN_time_tick,   &COUNTDOWN_start_pause,   &COUNTDOWN_abrt,    NULL,               &COUNTDOWN_exit},
    [PAUSE]     = { &PAUSE_inc_time,    &PAUSE_dec_time,      NULL,                   &PAUSE_start_pause,       &PAUSE_abrt,        &PAUSE_entry,       &PAUSE_exit},
    [STAT]      = { NULL,               NULL,                 &STAT_time_tick,        NULL,                     NULL,               &STAT_entry,        &STAT_exit}
  };

  mobj->state_table = (uintptr_t*)(&protimer_state_table[0][0]);
}

static void display_init( void )
{
  lcd_begin( 16, 2);
  lcd_clear();
  lcd_move_cursor_L_to_R();
  lcd_set_cursor( 0, 0);
  lcd_no_auto_scroll();
  lcd_cursor_off();
}