#include "main.h"
#include "lcd.h"

/* Private Function Prototypes */
static void do_beep( void );
static void display_clear( void );
static void display_message( String s,uint8_t c , uint8_t r );
static void display_time( uint32_t time );

static event_status_t protimer_state_handler_IDLE( protimer_t  *const mobj, event_t const *const e );
static event_status_t protimer_state_handler_TIME_SET( protimer_t  *const mobj, event_t const *const e );
static event_status_t protimer_state_handler_COUNTDOWN( protimer_t  *const mobj, event_t const *const e );
static event_status_t protimer_state_handler_PAUSE( protimer_t  *const mobj, event_t const *const e );
static event_status_t protimer_state_handler_STAT( protimer_t  *const mobj, event_t const *const e );

/* Public Function Definitions */
void protimer_init( protimer_t *mobj )
{
  event_t entry_action;
  entry_action.sig = ENTRY;
  mobj->active_state = IDLE;
  mobj->pro_time = 0u;
  protimer_state_machine( mobj, &entry_action );
}

event_status_t protimer_state_machine( protimer_t *const mobj,  event_t const *const e )
{
  switch( mobj->active_state )
  {
    case IDLE:
    {
      Serial.println("IDLE STATE");
      return protimer_state_handler_IDLE( mobj, e );
    }
    case TIME_SET:
    {
      Serial.println("TIME SET STATE");
      return protimer_state_handler_TIME_SET( mobj, e );
    }
    case COUNTDOWN:
    {
      Serial.println("COUNTDOWN STATE");
      return protimer_state_handler_COUNTDOWN( mobj, e);
    }
    case PAUSE:
    {
      Serial.println("PAUSE STATE");
      return protimer_state_handler_PAUSE( mobj, e);
    }
    case STAT:
    {
      Serial.println("STATUS STATE");
      return protimer_state_handler_STAT( mobj, e);
    }
  }
  return EVENT_IGNORED;
}

/* Private Function Definitions */
static event_status_t protimer_state_handler_IDLE( protimer_t *const mobj, event_t const *const e )
{
  switch( e->sig )
  {
    case ENTRY:
    {
      mobj->curr_time = 0;
      mobj->elapsed_time = 0;
      display_time(0);
      display_message("Set", 0, 0);
      display_message("time", 0, 1);
      return EVENT_HANDLED;
    }
    case EXIT:
    {
      display_clear();
      return EVENT_HANDLED;
    }
    case INC_TIME:
    {
      mobj->curr_time += 60;
      mobj->active_state = TIME_SET;
      return EVENT_TRANSITION;
    }
    case START_PAUSE:
    {
      mobj->active_state = STAT;
      return EVENT_TRANSITION;
    }
    case TIME_TICK:
    {
      /* turn on the buzzer every 500ms */
      if( ((protimer_tick_event_t *)(e))->ss == 5)
      {
        do_beep();
        return EVENT_HANDLED;
      }
      return EVENT_IGNORED;
    }
  } /* End of Switch */

  return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_TIME_SET(protimer_t *const mobj, event_t const *const e)
{
  switch( e->sig )
  {
    case ENTRY:
    {
      display_time( mobj->curr_time );
      return EVENT_HANDLED;
    }
    case EXIT:
    {
      display_clear();
      return EVENT_HANDLED;
    }
    case INC_TIME:
    {
      mobj->curr_time += 60;
      display_time( mobj->curr_time );
      return EVENT_HANDLED;
    }
    case DEC_TIME:
    {
      if( mobj->curr_time >=60 )
      {
        mobj->curr_time -= 60;
        display_time( mobj->curr_time );
        return EVENT_HANDLED;
      }
      return EVENT_IGNORED;
    }
    case ABRT:
    {
      mobj->active_state = IDLE;
      return EVENT_TRANSITION;
    }
    case START_PAUSE:
    {
      if( mobj->curr_time >= 60 )
      {
        mobj->active_state = COUNTDOWN;
        return EVENT_TRANSITION;
      }
      return EVENT_IGNORED;
    }
  }
  return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_COUNTDOWN( protimer_t *const mobj, event_t const *const e )
{
  switch(e->sig)
  {
    case EXIT:
    {
      mobj->pro_time += mobj->elapsed_time;
      mobj->elapsed_time = 0;
      return EVENT_HANDLED;
    }

    case TIME_TICK:
    {
      if( ((protimer_tick_event_t*)(e))->ss == 10 )
      {
        --mobj->curr_time;
        ++mobj->elapsed_time;
        display_time(mobj->curr_time);
        if( !mobj->curr_time )
        {
          mobj->active_state = IDLE;
          return EVENT_TRANSITION;
        }
        return EVENT_HANDLED;
      }
      return EVENT_IGNORED;
    }
    case START_PAUSE:
    {
      mobj->active_state = PAUSE;
      return EVENT_TRANSITION;
    }
    case ABRT:
    {
      mobj->active_state = IDLE;
      return EVENT_TRANSITION;
    }
  }
  return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_PAUSE( protimer_t *const mobj, event_t const *const e )
{
  switch(e->sig)
  {
    case ENTRY:
    {
      display_message("Paused", 5, 1);
      return EVENT_HANDLED;
    }
    case EXIT:
    {
      display_clear();
      return EVENT_HANDLED;
    }
    case INC_TIME:
    {
      mobj->curr_time += 60;
      mobj->active_state = TIME_SET;
      return EVENT_TRANSITION;
    }
    case DEC_TIME:
    {
      if(mobj->curr_time >= 60)
      {
        mobj->curr_time -= 60;
        mobj->active_state = TIME_SET;
        return EVENT_TRANSITION;
      }
      return EVENT_IGNORED;
    }
    case START_PAUSE:
    {
      mobj->active_state = COUNTDOWN;
      return EVENT_TRANSITION;
    }
    case ABRT:
    {
      mobj->active_state = IDLE;
      return EVENT_TRANSITION;
    }
  }
  return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_STAT( protimer_t *const mobj, event_t const *const e )
{
  static uint8_t tick_count;

  switch( e->sig )
  {
    case ENTRY:
    {
      display_time( mobj->pro_time );
      display_message("Productive time", 1, 1);
      return EVENT_HANDLED;
    }
    case EXIT:
    {
      display_clear();
      return EVENT_HANDLED;
    }
    case TIME_TICK:
    {
      tick_count++;
      if( tick_count == 30 )
      {
        tick_count = 0;
        mobj->active_state = IDLE;
        return EVENT_TRANSITION;
      }
      return EVENT_IGNORED;
    }
  }
  return EVENT_IGNORED;
}

/* Some Helper Functions */
static void display_time( uint32_t time )
{
  char buf[7];
  String time_msg;
  
  uint16_t minutes = time / 60;
  uint8_t seconds = time % 60;
  sprintf(buf,"%03d:%02d", minutes, seconds );
  
  time_msg = (String)buf;
  lcd_set_cursor( 5, 0 );
  lcd_print_string( time_msg );
}

static void display_message( String s, uint8_t c , uint8_t r )
{
  lcd_set_cursor( c, r);
  lcd_print_string( s );
}

static void display_clear(void)
{
  lcd_clear();
}

static void do_beep(void)
{
  /* buzzer frequency is 2Khz, and run the buzzer for 200 ms */
  tone(PIN_BUZZER, 2000, 200);
}
