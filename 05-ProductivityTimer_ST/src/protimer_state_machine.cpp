#include "main.h"
#include "lcd.h"

/* Private Function Prototypes */
static void do_beep( void );
static void display_clear( void );
static void display_message( String s,uint8_t c , uint8_t r );
static void display_time( uint32_t time );

/* Public Function Definitions */
void protimer_init( protimer_t *mobj )
{
  event_t entry_action;
  entry_action.sig = ENTRY;
  mobj->active_state = IDLE;
  mobj->pro_time = 0u;
}

/* Productive Timer Project All Event Handlers Function */
event_status_t IDLE_entry( protimer_t *const mobj, event_t const *const e )
{
  mobj->curr_time = 0;
  mobj->elapsed_time = 0;
  display_time(0);
  display_message("Set", 0, 0);
  display_message("time", 0, 1);
  return EVENT_HANDLED;
}

event_status_t IDLE_exit( protimer_t *const mobj, event_t const *const e )
{
  display_clear();
  return EVENT_HANDLED;
}

event_status_t IDLE_inc_time( protimer_t *const mobj, event_t const *const e )
{
  mobj->curr_time += 60;
  mobj->active_state = TIME_SET;
  return EVENT_TRANSITION;
}

event_status_t IDLE_pause( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = STAT;
  return EVENT_TRANSITION;
}

event_status_t IDLE_time_tick( protimer_t *const mobj, event_t const *const e )
{
  /* turn on the buzzer every 500ms */
  if( ((protimer_tick_event_t *)(e))->ss == 5)
  {
    do_beep();
    return EVENT_HANDLED;
  }
  return EVENT_IGNORED;
}

event_status_t TIME_SET_entry( protimer_t *const mobj, event_t const *const e )
{
  display_time( mobj->curr_time );
  return EVENT_HANDLED;
}

event_status_t TIME_SET_exit( protimer_t *const mobj, event_t const *const e )
{
  display_clear();
  return EVENT_HANDLED;
}

event_status_t TIME_SET_inc_time( protimer_t *const mobj, event_t const *const e )
{
  mobj->curr_time += 60;
  display_time( mobj->curr_time );
  return EVENT_HANDLED;
}

event_status_t TIME_SET_dec_time( protimer_t *const mobj, event_t const *const e )
{
  if( mobj->curr_time >=60 )
  {
    mobj->curr_time -= 60;
    display_time( mobj->curr_time );
    return EVENT_HANDLED;
  }
  return EVENT_IGNORED;
}

event_status_t TIME_SET_abrt( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = IDLE;
  return EVENT_TRANSITION;
}

event_status_t TIME_SET_start_pause( protimer_t *const mobj, event_t const *const e )
{
  if( mobj->curr_time >= 60 )
  {
    mobj->active_state = COUNTDOWN;
    return EVENT_TRANSITION;
  }
  return EVENT_IGNORED;
}

event_status_t COUNTDOWN_exit( protimer_t *const mobj, event_t const *const e )
{
  mobj->pro_time += mobj->elapsed_time;
  mobj->elapsed_time = 0;
  return EVENT_HANDLED;
}

event_status_t COUNTDOWN_time_tick( protimer_t *const mobj, event_t const *const e )
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

event_status_t COUNTDOWN_start_pause( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = PAUSE;
  return EVENT_TRANSITION;
}

event_status_t COUNTDOWN_abrt( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = IDLE;
  return EVENT_TRANSITION;
}

event_status_t PAUSE_entry( protimer_t *const mobj, event_t const *const e )
{
  display_message("Paused", 5, 1);
  return EVENT_HANDLED;
}

event_status_t PAUSE_exit( protimer_t *const mobj, event_t const *const e )
{
  display_clear();
  return EVENT_HANDLED;
}

event_status_t PAUSE_inc_time( protimer_t *const mobj, event_t const *const e )
{
  mobj->curr_time += 60;
  mobj->active_state = TIME_SET;
  return EVENT_TRANSITION;
}

event_status_t PAUSE_dec_time( protimer_t *const mobj, event_t const *const e )
{
  if(mobj->curr_time >= 60)
  {
    mobj->curr_time -= 60;
    mobj->active_state = TIME_SET;
    return EVENT_TRANSITION;
  }
  return EVENT_IGNORED;
}

event_status_t PAUSE_start_pause( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = COUNTDOWN;
  return EVENT_TRANSITION;
}

event_status_t PAUSE_abrt( protimer_t *const mobj, event_t const *const e )
{
  mobj->active_state = IDLE;
  return EVENT_TRANSITION;
}

event_status_t STAT_inc_time( protimer_t *const mobj, event_t const *const e )
{
  display_time( mobj->pro_time );
  display_message("Productive time", 1, 1);
  return EVENT_HANDLED;
}

event_status_t STAT_exit( protimer_t *const mobj, event_t const *const e )
{
  display_clear();
  return EVENT_HANDLED;
}

event_status_t STAT_time_tick( protimer_t *const mobj, event_t const *const e )
{
  static uint8_t tick_count;
  tick_count++;
  if( tick_count == 30 )
  {
    tick_count = 0;
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
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
