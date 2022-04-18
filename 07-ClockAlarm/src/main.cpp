#include <Arduino.h>
#include "qpn.h"
#include "lcd.h"
#include "ClockAlarm_SM.h"
Q_DEFINE_THIS_FILE;

/* Macros for buttons and buzzer pins */
#define PIN_BUTTON1           2u
#define PIN_BUTTON2           3u
#define PIN_BUZZER            12u

#define BTN_PAD_VALUE_SET     2u
#define BTN_PAD_VALUE_OK      1u
#define BTN_PAD_VALUE_ABRT    3u

/* enum for button states */
typedef enum
{
  NOT_PRESSED,
  BOUNCE,
  PRESSED
} Button_State_t;

/* Private Function Prototypes */
static void Timer1_Setup( void );
static void display_init( void );
static uint8_t process_button_pad_value( uint8_t button_pad_value );

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Clock Alarm Project Starting..");
  display_init();
  Clock_Alarm_ctor();
  QHSM_INIT( super_ClockAlarm );
  Timer1_Setup();
}

void loop() 
{
  static uint32_t tick_time = millis();
  uint8_t button1, button2, button_pad_value;

  /* Send tick event every 50ms */
  if( millis() - tick_time >= 50u )
  {
    tick_time = millis();
    Q_SIG( super_ClockAlarm) = TICK_SIG;
    QHSM_DISPATCH( super_ClockAlarm );
  }

  /* Read the Button Status */
  button1 = digitalRead( PIN_BUTTON1 );
  button2 = digitalRead( PIN_BUTTON2 );

  button_pad_value = (button1<<1u) | (button2<<0u);
  /* Software Button Debouncing */
  button_pad_value = process_button_pad_value( button_pad_value );
  /* Send/Dispatch Signal based on the pressed buttons */
  if( button_pad_value )
  {
    if( button_pad_value == BTN_PAD_VALUE_SET)
    {
      Q_SIG(super_ClockAlarm) = SET_SIG;
    }
    else if( button_pad_value == BTN_PAD_VALUE_OK)
    {
      Q_SIG(super_ClockAlarm) = OK_SIG;
    }
    else if( button_pad_value == BTN_PAD_VALUE_ABRT )
    {
      Q_SIG(super_ClockAlarm) = ABRT_SIG;
    }
    else
    {
      Q_SIG(super_ClockAlarm) = IGNORE_SIG;
    }
      
    QHSM_DISPATCH(super_ClockAlarm);
  }
}

/* Private Function Definitions */
static void Timer1_Setup( void )
{
  TCCR1A = 0x00;        /* CTC Mode */
  TCCR1B = B00001100;   /* Prescaler=256, CTC Mode */
  TIMSK1 |= B00000010;  /* Interrupt Enable for OCR1A Compare Match */
  OCR1A = 6250-1;       /* OC Match value for 100ms time base generation */
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

static uint8_t process_button_pad_value( uint8_t button_pad_value )
{
  static Button_State_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time = millis();

  switch( btn_sm_state )
  {
    case NOT_PRESSED:
    {
      if( button_pad_value )
      {
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }
    case BOUNCE:
    {
      if(millis() - curr_time >= 50u )
      {
        //50ms has passed 
        if(button_pad_value)
        {
          btn_sm_state = PRESSED;
          return button_pad_value;
        }
        else
        {
          btn_sm_state = NOT_PRESSED;
        }
      }
      break;
    }
    case PRESSED:
    {
      if(!button_pad_value)
      {
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }

  }

  return 0;
}

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}
