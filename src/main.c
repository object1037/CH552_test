#include <8051.h>
#include <stdint.h>

void delay(void);

void main(void) {
  while (1) {
    P3_0 = 0;
    delay();
    P3_0 = 1;
    delay();
  }
}

void delay(void) {
  uint16_t i;

  for (i = 0; i < 0xffff; i++);
}
