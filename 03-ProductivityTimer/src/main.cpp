#include "main.h"

/* Private Function Prototypes */
static void protimer_event_dispatcher( protimer_t *const mobj, event_t const *const e );
static uint8_t process_button_pad_value( uint8_t btn_pad_value );
static void display_init( void );

/* Main Application Object */
static protimer_t protimer;

void setup() 
{
  pinMode( PIN_BUTTON1, INPUT );
  pinMode( PIN_BUTTON2, INPUT );
  pinMode( PIN_BUTTON3, INPUT );
  protimer_init( &protimer );
  display_init();
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
    ++tick_event.ss;
    if( tick_event.ss > 10 )
    {
      tick_event.ss = 1;
      /* dispatch the event */
      protimer_event_dispatcher( &protimer, &tick_event.super );
    }
  }
}

/* Private Function Definitions */
static void protimer_event_dispatcher( protimer_t *const mobj, event_t const *const e )
{
  event_status_t status;
  protimer_state_t source, target;
  
  source = mobj->active_state;
  status = protimer_state_machine( mobj, e );

  if(status == EVENT_TRANSITION)
  {
    target = mobj->active_state;
    event_t ee;
    /* run the exit action for the source state */
    ee.sig = EXIT;
    mobj->active_state = source;
    protimer_state_machine( mobj, &ee );

    /* run the entry action for the target state */
    ee.sig = ENTRY;
    mobj->active_state = target;
    protimer_state_machine( mobj , &ee );
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

static void display_init( void )
{
  lcd_begin( 16, 2);
  lcd_clear();
  lcd_move_cursor_L_to_R();
  lcd_set_cursor( 0, 0);
  lcd_no_auto_scroll();
  lcd_cursor_off();
}