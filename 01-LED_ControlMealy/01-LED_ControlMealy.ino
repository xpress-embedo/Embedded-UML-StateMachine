
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


void setup()
{
  Serial.begin(115200);
  Serial.println("Light control application");
  Serial.println("-------------------------");
  Serial.println("Send 'x' or 'o'");
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
  switch( current_state )
  {
    case LIGHT_OFF:
    {
      switch(event)
      {
        case EVENT_ON:
        {
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_DIM);
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
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_MEDIUM);
          current_state = LIGHT_MEDIUM;
          break;
        }
        case EVENT_OFF:{
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_OFF);
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
        case EVENT_ON:{
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_FULL);
          current_state = LIGHT_FULL;
          break;
        }
        case EVENT_OFF:
        {
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_OFF);
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
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_DIM);
          current_state = LIGHT_DIM;         
          break;
        }
        case EVENT_OFF:{
          light_change_intensity(PIN_LED,LIGHT_BRIGHT_OFF);
          current_state = LIGHT_OFF;          
          break;
        }
      }
      break;
    }
  } 
}

void light_change_intensity( uint8_t pin, uint8_t intensity )
{
  analogWrite(pin, intensity);
}
