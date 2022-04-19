#include <Arduino.h>
#include "main.h"
#include "qpn.h"
#include "lcd.h"
#include "ClockAlarm_SM.h"
#include "Button_SM.h"

Q_DEFINE_THIS_FILE;

/* Some Important Points about QF (Active Object Framework)
1) Create Event Queues (array of QEvt) for the active objects.
2) Create and Initialize Active Object Control Block (QActiveCB)
3) Call QF_init( maxActive ) to initialize the Active object framework of QP-nano
  * this also calls the init funcvtion of underlying kernel.
4) Call QF_run()
  * this also calls the scheduler of the kernel and never returns
5) Call QF_tickXISR() from your application's tick ISR
*/

/* Private Function Prototypes */
static void sys_tick_init( void );
static void display_init( void );
static void attach_button_interrupts( void );
static void SET_Handler( void );
static void OK_Handler( void );

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location );

uint8_t button_report_flag = 0x01;

/* 1) Create Event Queues (array of QEvt) for the active objects. */
static QEvt ClockAlarmQueue[8];
static QEvt ButtonQueue[8];

/* 2) Create and Initialize Active Object Control Block (QActiveCB) */
QActiveCB const QF_active[] = 
{
  /* the first entry should be a NULL entry */
  { (QActive*)0,              (QEvt*)0 ,                0 },
  { (QActive*)AO_ClockAlarm,  (QEvt*)ClockAlarmQueue,   Q_DIM(ClockAlarmQueue) },
  { (QActive*)AO_Button,      (QEvt*)ButtonQueue,       Q_DIM(ButtonQueue) }
};

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Clock Alarm Project Starting..");
  display_init();
  attach_button_interrupts();
  Clock_Alarm_ctor();
  Button_ctor();
  /* 3) Call QF_init( maxActive ) to initialize the Active object framework of QP-nano */
  QF_init( Q_DIM(QF_active) );
}

void loop() 
{
  /* 4) Call QF_run() (this must be called inside a loop function.
        It executes the events of the active objects in the cooperative manner.
        If there are no events then it execute the QV_onIdle function.
        This QV_onIdle we must define in our project, here we can mention the
        code like sleep mode.
  */
  QF_run();
}

void QF_onStartup( void )
{
  sys_tick_init();
}

void QV_onIdle( void )
{
  QV_CPU_SLEEP();
}

/* Private Function Definitions */
static void sys_tick_init( void )
{
  TCCR1A = TCCR1A_CTC_MODE;                           /* CTC Mode */
  TCCR1B = (TCCR1B_CTC_MODE | TCCR1B_PRESCALER_1);    /* Prescaler=1, CTC Mode */
  TIMSK1 |= B00000010;                                /* Interrupt Enable for OCR1A Compare Match */
  OCR1A = TIMER1_OC_MATCH_VALUE;                      /* OC Match value for 100ms time base generation */
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

static void attach_button_interrupts( void )
{
  attachInterrupt( digitalPinToInterrupt(PIN_BUTTON1), SET_Handler, RISING );
  attachInterrupt( digitalPinToInterrupt(PIN_BUTTON2), OK_Handler, RISING );
}

void SET_Handler( void )
{
  /* This macro is provided by QPN Framework, we can also use noInterrupt and interrupts function */
  QF_INT_DISABLE();
  if( button_report_flag )
  {
    button_report_flag = 0x00;
    QActive_armX( AO_Button, 0, MS_TO_TICKS(30), 0 );
  }
  QF_INT_ENABLE();
}

void OK_Handler( void )
{
  QF_INT_DISABLE();
  if( button_report_flag )
  {
    button_report_flag = 0x00;
    QActive_armX( AO_Button, 0, MS_TO_TICKS(30), 0 );
  }
  QF_INT_ENABLE();
}

Q_NORETURN Q_onAssert	(	char_t const *const 	module, int_t const 	location )
{
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  /* As per documentation it's not a good idea to do like this */
  while(1);
}
