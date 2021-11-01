#define FAKE_EPOCH_RETURN_VALUE 100
#define STUB_PROGRESSIVE_INTERVAL 1000

static void timerHelperSetup();
static unsigned long fake_epoch_ms_fn();
static unsigned long progressive_ms;
static unsigned long stub_progressive_epoch_ms_fn();

unsigned long fake_epoch_ms_fn() {
  return FAKE_EPOCH_RETURN_VALUE;
}
unsigned long stub_progressive_epoch_ms_fn() {
  // It returns 1 for the Timer_Start and 1001 for the Timer_GetMillis
  return progressive_ms += STUB_PROGRESSIVE_INTERVAL;
}

void timerHelperSetup() {
  progressive_ms = 1;
}