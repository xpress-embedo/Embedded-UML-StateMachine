/*.$file${AOs::../src::ClockAlarm_SM.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: ClockAlarmAO.qm
* File:  ${AOs::../src::ClockAlarm_SM.h}
*
* This code has been generated by QM 5.1.1 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${AOs::../src::ClockAlarm_SM.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#ifndef CLOCK_ALARM_H
#define CLOCK_ALARM_H

enum ClockAlarm_Signals
{
  SET_SIG = Q_USER_SIG,    /* Q_USER_SIG marks the beginning of User Signals */
  OK_SIG,
  ABRT_SIG,
  ALARM_SIG,
  TICK_SIG,
  ALARM_CHECK_SIG,
  BUTTON_SCAN_SIG,
  IGNORE_SIG,
  MAX_SIG
};

enum time_mode
{
  TIME_MODE_24H = 0,
  TIME_MODE_12H,
};

enum alarm_status
{
  ALARM_OFF = 0,
  ALARM_ON
};

enum setting
{
  NO_SETTING = 0,
  CLOCK_SETTING,
  ALARM_SETTING
};

typedef enum time_format
{
  FORMAT_24H = 0,
  FORMAT_AM,
  FORMAT_PM,
} time_format_t;

typedef enum
{
  NOT_PRESSED,
  BOUNCE,
  PRESSED
} Button_State_t;

#define INITIAL_CURRENT_TIME    (((10ul*3600ul) + (10ul*60ul) + 10ul) * 10ul)    /* 10 hours 10 minutes 10 seconds */
#define INITIAL_ALARM_TIME      (8ul*3600ul)                                     /* 8 hours */
#define MAX_TIME                (864000ul)                                       /* 24*3600*10 */
/* Display Related Macros */
#define TICKING_CURR_TIME_ROW             (0u)
#define TICKING_CURR_TIME_COL             (3u)
#define CLOCK_SETTING_TIME_ROW            (0u)
#define CLOCK_SETTING_TIME_COL            (2u)
#define CLOCK_SETTING_TIME_HOUR_D1_COL    (2u)
#define CLOCK_SETTING_TIME_HOUR_D2_COL    (3u)
#define CLOCK_SETTING_TIME_MIN_D1_COL     (5u)
#define CLOCK_SETTING_TIME_MIN_D2_COL     (6u)
#define CLOCK_SETTING_TIME_SEC_D1_COL     (8u)
#define CLOCK_SETTING_TIME_SEC_D2_COL     (9u)
#define CLOCK_SETTING_TIME_FMT_COL        (11u)

#define CLOCK_SETTING_ERR_MSG_ROW         (2u)
#define CLOCK_SETTING_ERR_MSG_COL         (4u)
#define CLOCK_SETTING_ERR_MSG_COL_END     (9u)

#define ALARM_SETTING_CURR_TIME_ROW       (1u)
#define ALARM_SETTING_CURR_TIME_COL       (2u)
#define ALARM_SETTING_STATUS_ROW          (0u)
#define ALARM_SETTING_STATUS_COL          (4u)
#define ALARM_NOTIFY_MSG_ROW              (0u)
#define ALARM_NOTIFY_MSG_COL              (4u)

/*.$declare${AOs::AO_ClockAlarm} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
extern QActive *const AO_ClockAlarm;
/*.$enddecl${AOs::AO_ClockAlarm} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$declare${AOs::Clock_Alarm_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Clock_Alarm_ctor} ................................................*/
void Clock_Alarm_ctor(void);
/*.$enddecl${AOs::Clock_Alarm_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#endif