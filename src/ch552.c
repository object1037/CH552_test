#include "ch552.h"
#include <8051.h>

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
    // delayMicroseconds(1000);
    while ((TKEY_CTRL & bTKC_IF) == 0);
    while (TKEY_CTRL & bTKC_IF);
    ms--;
  }
}

void delayMicroseconds(uint16_t us) {
  TMOD |= 0x02;
  T2MOD |= bT0_CLK;  // 2/3 us per tick
  if (us > 170) {
    TL0 = 1;
    TH0 = 1;
    TR0 = 1;
    while (us > 170) {
      while (!TF0);
      TF0 = 0;
      us -= 170;
    }
    TR0 = 0;
  }

  TL0 = 0x100 - us * 1.5;
  TR0 = 1;
  while (!TF0);
  TF0 = 0;
  TR0 = 0;
}
