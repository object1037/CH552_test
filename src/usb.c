#include "usb.h"
#include "ch552.h"

USB_DEV_DESCR DevDesc = {
  .bLength = sizeof(USB_DEV_DESCR),
  .bDescriptorType = USB_DESCR_TYP_DEVICE,
  .bcdUSBL = 0x00,
  .bcdUSBH = 0x02,  // USB 2.0
  .bDeviceClass = USB_DEV_CLASS_COMMUNIC,
  .bDeviceSubClass = 0x00,
  .bDeviceProtocol = 0x00,
  .bMaxPacketSize0 = DEFAULT_ENDP0_SIZE,
  .idVendorL = 0x86,
  .idVendorH = 0x1a,
  .idProductL = 0x23,
  .idProductH = 0x55,
  .bcdDeviceL = 0x00,
  .bcdDeviceH = 0x01,
  .iManufacturer = 0x00,
  .iProduct = 0x00,
  .iSerialNumber = 0x00,
  .bNumConfigurations = 0x01,
};

USB_CFG_DESCR_LONG CfgDesc = {
  .cfg_descr = {
    .bLength = sizeof(USB_CFG_DESCR),
    .bDescriptorType = sizeof(USB_CFG_DESCR_LONG),
    .wTotalLengthL = 0x20,
    .wTotalLengthH = 0x00,
    .bNumInterfaces = 0x01,
    .bConfigurationValue = 0x01,
    .iConfiguration = 0x00,
    .bmAttributes = 0x80,
    .MaxPower = 0x32,  // 100 mA
  },
  .itf_descr = {
    .bLength = sizeof(USB_ITF_DESCR),
    .bDescriptorType = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber = 0x00,
    .bAlternateSetting = 0x00,
    .bNumEndpoints = 0x01,
    .bInterfaceClass = USB_DEV_CLASS_COMMUNIC,
    .bInterfaceSubClass = 0x00,
    .bInterfaceProtocol = 0x00,
    .iInterface = 0x00,
  },
  .endp_descr = {{
    .bLength = sizeof(USB_ENDP_DESCR),
    .bDescriptorType = USB_DESCR_TYP_ENDP,
    .bEndpointAddress = 0x81,  // EP1 IN
    .bmAttributes = 0x03,      // interrupt
    .wMaxPacketSizeL = 0x08,
    .wMaxPacketSizeH = 0x00,
    .bInterval = 0xFF,
  }},
};

__xdata __at(0x0000) uint8_t Ep0Buffer[DEFAULT_ENDP0_SIZE];
__xdata __at(0x0008) uint8_t Ep1Buffer[DEFAULT_ENDP0_SIZE];

void USBDevConfig(void) {
  USB_CTRL = 0x00;
  USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;
  USB_DEV_AD = 0x00;
  USB_CTRL &= ~bUC_LOW_SPEED;
  UDEV_CTRL &= ~bUD_LOW_SPEED;
  UDEV_CTRL |= bUD_PD_DIS | bUD_PORT_EN;
}

void USBIntConfig(void) {
  USB_INT_EN |= bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
  USB_INT_FG |= 0x1F;
  IE_USB = 1;
  EA = 1;
}

void USBDevEPConfig(void) {
  UEP1_DMA = (uint16_t)Ep1Buffer;
  UEP4_1_MOD |= bUEP1_TX_EN;
  UEP4_1_MOD &= ~bUEP1_BUF_MOD;
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;

  UEP0_DMA = (uint16_t)Ep0Buffer;
  UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
}

void USBInterrupt(void) __interrupt(INT_NO_USB) {

}
