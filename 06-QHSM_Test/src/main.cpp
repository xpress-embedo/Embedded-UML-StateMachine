#include <Arduino.h>
#include "qpn.h"
#include "QHSM_Test.h"
Q_DEFINE_THIS_FILE;

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("QHSM Testing....");
  QHsmTst_ctor();
  QHSM_INIT( super_QHsmTst );
  Serial.println();
}

void loop() 
{
  char user_data;
  if( Serial.available() > 0 )
  {
    user_data = Serial.read();
    if( user_data == 'A')
    {
      /* super_QHsmTst->evt.sig = (QSignal)A_SIG; // Below line is also same */
      Q_SIG( super_QHsmTst ) = (QSignal)A_SIG;
    }
    else if( user_data == 'B' )   { Q_SIG(super_QHsmTst) = (QSignal)B_SIG; }
    else if( user_data == 'C' )   { Q_SIG(super_QHsmTst) = (QSignal)C_SIG; }
    else if( user_data == 'D' )   { Q_SIG(super_QHsmTst) = (QSignal)D_SIG; }
    else if( user_data == 'E' )   { Q_SIG(super_QHsmTst) = (QSignal)E_SIG; }
    else if( user_data == 'F' )   { Q_SIG(super_QHsmTst) = (QSignal)F_SIG; }
    else if( user_data == 'G' )   { Q_SIG(super_QHsmTst) = (QSignal)G_SIG; }
    else if( user_data == 'H' )   { Q_SIG(super_QHsmTst) = (QSignal)H_SIG; }
    else if( user_data == 'I' )   { Q_SIG(super_QHsmTst) = (QSignal)I_SIG; }
    else if( user_data == 'X' )   { Q_SIG(super_QHsmTst) = (QSignal)TERMINATE_SIG; }
    else { Q_SIG(super_QHsmTst) = IGNORE_SIG; }

    QHSM_DISPATCH( super_QHsmTst );
    Serial.println();
  }
}


Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}