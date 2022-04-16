#include "main.h"


void protimer_init( protimer_t *mobj )
{
  mobj->active_state = IDLE;
  mobj->pro_time = 0u;
}