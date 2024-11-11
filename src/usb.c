#include "usb.h"

#include <string.h>
#include "ch552.h"

__code USB_DEV_DESCR DevDesc = {
  .bLength = sizeof(USB_DEV_DESCR),
  .bDescriptorType = USB_DESCR_TYP_DEVICE,
  .bcdUSBL = 0x10,
  .bcdUSBH = 0x01,  // USB 1.1
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

__code USB_CFG_DESCR_CDC CfgDesc = {
  .cfg_descr = {
    .bLength = sizeof(USB_CFG_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CONFIG,
    .wTotalLengthL = 0x43,
    .wTotalLengthH = 0x00,
    .bNumInterfaces = 0x02,
    .bConfigurationValue = 0x01,
    .iConfiguration = 0x00,
    .bmAttributes = 0x80,
    .MaxPower = 0x32,  // 100 mA
  },
  .comm_itf_descr = {
    .bLength = sizeof(USB_ITF_DESCR),
    .bDescriptorType = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber = 0x00,
    .bAlternateSetting = 0x00,
    .bNumEndpoints = 0x01,
    .bInterfaceClass = USB_INTF_CLASS_COMMUNIC,
    .bInterfaceSubClass = USB_INTF_SUBCLASS_ACM,
    .bInterfaceProtocol = 0x00,
    .iInterface = 0x00,
  },
  .header_func_descr = {
    .bFunctionLength = sizeof(USB_CDC_HEADER_FUNC_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = USB_DESCR_SUBTYP_HEADER,
    .bcdCDCL = 0x10,
    .bcdCDCH = 0x01,
  },
  .acm_func_descr = {
    .bFunctionLength = sizeof(USB_CDC_ACM_FUNC_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = USB_DESCR_SUBTYP_ACM,
    .bmCapabilities = 0x0F,
  },
  .union_func_descr = {
    .bFunctionLength = sizeof(USB_CDC_UNION_FUNC_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = USB_DESCR_SUBTYP_UNION,
    .bControlInterface = 0x00,       // communication interface
    .bSubordinateInterface0 = 0x01,  // data interface
  },
  .call_func_descr = {
    .bFunctionLength = sizeof(USB_CDC_CALL_FUNC_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = USB_DESCR_SUBTYP_CALL,
    .bmCapabilities = 0x03,
    .bDataInterface = 0x01,
  },
  .comm_endp_descr = {{
    .bLength = sizeof(USB_ENDP_DESCR),
    .bDescriptorType = USB_DESCR_TYP_ENDP,
    .bEndpointAddress = 0x81,  // EP1 IN
    .bmAttributes = 0x03,      // interrupt
    .wMaxPacketSizeL = 0x08,
    .wMaxPacketSizeH = 0x00,
    .bInterval = 0xFF,
  }},
  .data_itf_descr = {
    .bLength = sizeof(USB_ITF_DESCR),
    .bDescriptorType = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber = 0x01,
    .bAlternateSetting = 0x00,
    .bNumEndpoints = 0x02,
    .bInterfaceClass = USB_INTF_CLASS_DATA,
    .bInterfaceSubClass = 0x00,
    .bInterfaceProtocol = 0x00,
    .iInterface = 0x00,
  },
  .data_endp_descr = {{
    .bLength = sizeof(USB_ENDP_DESCR),
    .bDescriptorType = USB_DESCR_TYP_ENDP,
    .bEndpointAddress = 0x02,  // EP2 OUT
    .bmAttributes = 0x02,      // bulk
    .wMaxPacketSizeL = 0x40,
    .wMaxPacketSizeH = 0x00,
    .bInterval = 0x00,
  },{
    .bLength = sizeof(USB_ENDP_DESCR),
    .bDescriptorType = USB_DESCR_TYP_ENDP,
    .bEndpointAddress = 0x82,  // EP2 IN
    .bmAttributes = 0x02,      // bulk
    .wMaxPacketSizeL = 0x40,
    .wMaxPacketSizeH = 0x00,
    .bInterval = 0x00,
  }},
};

__xdata __at(0x0000) uint8_t Ep0Buffer[DEFAULT_ENDP0_SIZE];
__xdata __at(0x0008) uint8_t Ep1Buffer[MAX_PACKET_SIZE];
__xdata __at(0x0048) uint8_t Ep2Buffer[2 * MAX_PACKET_SIZE];

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
  UEP0_DMA = (uint16_t)Ep0Buffer;
  UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
}

void SendData(uint8_t *buf) {}

void USBInterrupt(void) __interrupt(INT_NO_USB) __using(1) {}
