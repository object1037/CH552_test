#include "ch552.h"
#include "time.h"
#include "usb.h"

void USBInterrupt(void) __interrupt(INT_NO_USB) __using(1);

void main(void) {
  ConfigClock();
  delay(50);

  USBDevConfig();
  USBDevEPConfig();
  USBIntConfig();
  UEP0_T_LEN = 0;
  UEP1_T_LEN = 0;
  UEP2_T_LEN = 0;

  while (1) {
    // SendData("Hello, World!\r\n");
    SendKeycode(0x04);
    delay(50);
    SendKeycode(0x00);
    delay(5000);
  }
}
