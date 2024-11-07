#include "ch552.h"

void ConfigClock(void) {
  // enter safe mode
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;

  CLOCK_CFG |= bOSC_EN_XT;
  delay(10);  // wait for the clock to stabilize
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  CLOCK_CFG &= ~bOSC_EN_INT;

  // leave safe mode
  SAFE_MOD = 0x00;
}

void delay(uint16_t ms) {
  while (ms) {
    delayMicroseconds(1000);
    --ms;
  }
}
void delayMicroseconds(uint16_t us) {
  while (us) {
    --us;
  }
}
