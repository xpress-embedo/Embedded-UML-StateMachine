#include <Arduino.h>
#include "qpn.h"
#include "ClockAlarm_SM.h"
Q_DEFINE_THIS_FILE;

/* Private Function Prototypes */
Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Clock Alarm Project Starting..");
}

void loop() 
{
}

/* Private Function Definitions */
Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}
