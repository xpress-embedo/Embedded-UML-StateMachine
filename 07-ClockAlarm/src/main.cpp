#include <Arduino.h>
#include "qpn.h"
#include "lcd.h"
#include "ClockAlarm_SM.h"
Q_DEFINE_THIS_FILE;

/* Private Function Prototypes */
static void Timer1_Setup( void );
static void display_init( void );
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

  /* Send tick event every 50ms */
  if( millis() - tick_time >= 50u )
  {
    tick_time = millis();
    Q_SIG( super_ClockAlarm) = TICK_SIG;
    QHSM_DISPATCH( super_ClockAlarm );
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

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}
