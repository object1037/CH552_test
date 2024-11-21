#include "ch552.h"
#include "time.h"
#include "usb.h"

void USBInterrupt(void) __interrupt(INT_NO_USB) __using(1);

SBIT(COL0, 0x90, 1);  // P1.1
SBIT(COL1, 0xB0, 1);  // P3.1
SBIT(ROW0, 0xB0, 4);  // P3.4
SBIT(ROW1, 0xB0, 5);  // P3.5

void SetupPin(void) {
  P1_MOD_OC &= ~0x02;
  P1_DIR_PU |= 0x02;
  P3_MOD_OC |= 0x02;
  P3_MOD_OC &= ~(0x10 | 0x20);
  P3_DIR_PU |= 0x02 | 0x10 | 0x20;
}

void main(void) {
  ConfigClock();
  SetupPin();
  delay(50);

  USBDevConfig();
  USBDevEPConfig();
  USBIntConfig();
  UEP0_T_LEN = 0;
  UEP1_T_LEN = 0;
  UEP2_T_LEN = 0;

  while (1) {
    COL0 = 0;
    if (ROW0 == 0) {
      SendKeycode(0x04);
    } else if (ROW1 == 0) {
      SendKeycode(0x05);
    } else {
      SendKeycode(0x00);
    }
    COL0 = 1;

    COL1 = 0;
    if (ROW0 == 0) {
      SendKeycode(0x06);
    } else if (ROW1 == 0) {
      SendKeycode(0x07);
    } else {
      SendKeycode(0x00);
    }
    COL1 = 1;
  }
}
