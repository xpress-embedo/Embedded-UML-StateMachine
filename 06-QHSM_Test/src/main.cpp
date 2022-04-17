#include <Arduino.h>
#include "qpn.h"
#include "QHSM_Test.h"

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

void setup()
{
  QHsmTst_ctor();
}

void loop() 
{
}


Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}