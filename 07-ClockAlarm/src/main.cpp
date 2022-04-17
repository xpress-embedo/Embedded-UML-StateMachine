#include <Arduino.h>
#include "qpn.h"
#include "ClockAlarm_SM.h"
Q_DEFINE_THIS_FILE;

/* Private Function Prototypes */
static void Timer1_Setup( void );
Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Timer1_Setup();
  Serial.println("Clock Alarm Project Starting..");
}

void loop() 
{
}

/* Private Function Definitions */
static void Timer1_Setup( void )
{
  TCCR1A = 0x00;        /* CTC Mode */
  TCCR1B = B00001100;   /* Prescaler=256, CTC Mode */
  TIMSK1 |= B00000010;  /* Interrupt Enable for OCR1A Compare Match */
  OCR1A = 62500-1;      /* OC Match value for 100ms time base generation */
}

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}
