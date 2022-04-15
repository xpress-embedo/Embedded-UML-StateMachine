
/* Pin/LED Under Action */
#define PIN_LED             9u
/* PWM Values for Different Brightness Settings */
#define LIGHT_BRIGHT_OFF    0u
#define LIGHT_BRIGHT_DIM    25u
#define LIGHT_BRIGHT_MEDIUM 85u
#define LIGHT_BRIGHT_FULL   255u

typedef enum _event
{
  EVENT_OFF = 0,
  EVENT_ON
} event;

typedef enum
{
  LIGHT_OFF = 0,
  LIGHT_DIM,
  LIGHT_MEDIUM,
  LIGHT_FULL
} light_state_e;

/* Current State of the State Machine */
static light_state_e current_state = LIGHT_OFF;

/* Private Function Prototypes */
void light_state_machine( uint8_t event );
void light_change_intensity( uint8_t pin, uint8_t intensity );
void run_entry_action( light_state_e state );

void setup()
{
  Serial.begin(115200);
  Serial.println("Light control application");
  Serial.println("-------------------------");
  Serial.println("Send 'x' or 'o'");
  run_entry_action( LIGHT_OFF );
}

void loop()
{
  uint8_t event;
  if(Serial.available() > 0)
  {
    event = Serial.read();
    if(event == 'o')
    {
      light_state_machine(EVENT_ON);
    }
    else if(event == 'x')
    {
      light_state_machine(EVENT_OFF);
    }
  }
}


void light_state_machine( uint8_t event )
{
  light_state_e previous_state;
  /* Save the current state */
  previous_state = current_state;
  
  switch( current_state )
  {
    case LIGHT_OFF:
    {
      switch(event)
      {
        case EVENT_ON:
        {
          current_state = LIGHT_DIM;
          break;
        }
      }
      break;
    }
    case LIGHT_DIM:
    {
      switch(event)
      {
        case EVENT_ON:
        {
          current_state = LIGHT_MEDIUM;
          break;
        }
        case EVENT_OFF:
        {
          current_state = LIGHT_OFF;
          break;
        }
      }
      break;
    }
    case LIGHT_MEDIUM:
    {
      switch(event)
      {
        case EVENT_ON:
        {
          current_state = LIGHT_FULL;
          break;
        }
        case EVENT_OFF:
        {
          current_state = LIGHT_OFF;         
          break;
        }
      }
      break;
    }
    case LIGHT_FULL:
    {
      switch(event)
      {
        case EVENT_ON:
        {
          current_state = LIGHT_DIM;         
          break;
        }
        case EVENT_OFF:
        {
          current_state = LIGHT_OFF;          
          break;
        }
      }
      break;
    }
  }

  /* In Moore State Machine, for each state we have an output
  and output is produced when state is changed.*/
  if( previous_state != current_state )
  {
    run_entry_action( current_state );
  }
}

void run_entry_action(light_state_e state)
{
  switch(state)
  {
    case LIGHT_OFF:
    {
      light_change_intensity(PIN_LED,LIGHT_BRIGHT_OFF);
      break; 
    }
    case LIGHT_DIM:
    {
      light_change_intensity(PIN_LED,LIGHT_BRIGHT_DIM);
      break; 
    }
    case LIGHT_MEDIUM:
    {
      light_change_intensity(PIN_LED,LIGHT_BRIGHT_MEDIUM);
      break; 
    }
    case LIGHT_FULL:
    {
      light_change_intensity(PIN_LED,LIGHT_BRIGHT_FULL);
      break; 
    }
  }
}

void light_change_intensity( uint8_t pin, uint8_t intensity )
{
  analogWrite(pin, intensity);
}
