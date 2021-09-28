//
// Created by jaume on 2/3/21.
//

static unsigned long (*epoch_millis)(void) = 0;
static unsigned long started_at;

void Timer_Create(const void *epoch_millis_fn) {
  epoch_millis = (unsigned long (*)(void)) epoch_millis_fn;
  started_at = 0;
}

void Timer_Start(void) {
  started_at = epoch_millis();
}

int Timer_IsRunning(void) {
  return 0 != started_at;
}

unsigned long Timer_GetMillis(void) {
  if (started_at == 0) return 0;
  return epoch_millis() - started_at;
}

void Timer_Destroy(void) {
  started_at = 0;
  epoch_millis = 0;
}
