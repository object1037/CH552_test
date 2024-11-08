#include <8051.h>
#include <stdint.h>

#include "ch552.h"

void main(void) {
  ConfigClock();

  while (1) {
    P3_0 = 0;
    delay(1000);
    P3_0 = 1;
    delay(1000);
  }
}
