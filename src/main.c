#include "ch552.h"
#include "time.h"

void main(void) {
  ConfigClock();

  while (1) {
    RXD = 0;
    delay(1000);
    RXD = 1;
    delay(1000);
  }
}
