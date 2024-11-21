#include "usb.h"

__code USB_DEV_DESCR DevDesc = {
  .bLength = sizeof(USB_DEV_DESCR),
  .bDescriptorType = USB_DESCR_TYP_DEVICE,
  .bcdUSBL = 0x10,
  .bcdUSBH = 0x01,  // USB 1.1
  .bDeviceClass = 0x00,
  .bDeviceSubClass = 0x00,
  .bDeviceProtocol = 0x00,
  .bMaxPacketSize0 = DEFAULT_ENDP0_SIZE,
  .idVendorL = 0xFE,
  .idVendorH = 0xCA,
  .idProductL = 0xBE,
  .idProductH = 0xBA,
  .bcdDeviceL = 0x00,
  .bcdDeviceH = 0x01,
  .iManufacturer = 0x01,
  .iProduct = 0x02,
  .iSerialNumber = 0x03,
  .bNumConfigurations = 0x01,
};

/*
__code USB_CFG_DESCR_CDC CfgDesc = {
  .cfg_descr = {
    .bLength = sizeof(USB_CFG_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CONFIG,
    .wTotalLengthL = sizeof(USB_CFG_DESCR_CDC),
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
    .bInterfaceProtocol = 0x01, // AT commands V.250
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
*/

__code USB_CFG_DESCR_HID CfgDesc = {
.cfg_descr = {
    .bLength = sizeof(USB_CFG_DESCR),
    .bDescriptorType = USB_DESCR_TYP_CONFIG,
    .wTotalLengthL = sizeof(USB_CFG_DESCR_HID),
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
    .bInterfaceClass = USB_INTF_CLASS_HID,
    .bInterfaceSubClass = 0x00, // boot device
    .bInterfaceProtocol = 0x01, // keyboard
    .iInterface = 0x00,
  },
  .hid_descr = {
    .bLength = sizeof(USB_HID_DESCR),
    .bDescriptorType = USB_DESCR_TYP_HID,
    .bcdHIDL = 0x11,
    .bcdHIDH = 0x01,
    .bCountryCode = 0x00,
    .bNumDescriptors = 0x01,
    .bDescriptorTypeX = USB_DESCR_TYP_REPORT,
    .wDescriptorLengthL = sizeof(ReportDesc),
    .wDescriptorLengthH = 0x00,
  },
  .endp_descr = {
    .bLength = sizeof(USB_ENDP_DESCR),
    .bDescriptorType = USB_DESCR_TYP_ENDP,
    .bEndpointAddress = 0x81,  // EP1 IN
    .bmAttributes = 0x03,      // interrupt
    .wMaxPacketSizeL = 0x08,
    .wMaxPacketSizeH = 0x00,
    .bInterval = 0x0A,         // 10 ms
  },
};

uint8_t __code ReportDesc[REPORT_SIZE] = {
  0x05, 0x01,  // G Usage Page (Generic Desktop Ctrls)
  0x09, 0x06,  // L Usage (Keyboard)
  0xA1, 0x01,  // M Collection (Application)

  0x05, 0x07,  // G  Usage Page (Kbrd/Keypad)
  0x19, 0xE0,  // L  Usage Minimum (0xE0 LCtrl)
  0x29, 0xE7,  // L  Usage Maximum (0xE7 RGUI)
  0x15, 0x00,  // G  Logical Minimum (0)
  0x25, 0x01,  // G  Logical Maximum (1)
  0x75, 0x01,  // G  Report Size (1bit)
  0x95, 0x08,  // G  Report Count (8)
  0x81, 0x02,  // M  Input (Var, modifier)

  0x75, 0x08,  // G  Report Size (8bit)
  0x95, 0x01,  // G  Report Count (1)
  0x81, 0x01,  // M  Input (Const, reserved)

  0x05, 0x07,  // G  Usage Page (Kbrd/Keypad)
  0x19, 0x00,  // L  Usage Minimum (0)
  0x29, 0x67,  // L  Usage Maximum (103)
  0x15, 0x00,  // G  Logical Minimum (0)
  0x25, 0x67,  // G  Logical Maximum (103)
  0x75, 0x08,  // G  Report Size (8bit)
  0x95, 0x06,  // G  Report Count (6)
  0x81, 0x00,  // M  Input (Array, keycodes)
  0xC0,        // M End Collection
};

unsigned char __code LangDesc[] = {
  sizeof(LangDesc),
  USB_DESCR_TYP_STRING,
  0x09, 0x04  // English (US)
};

unsigned char __code ManufDesc[] = {
  sizeof(ManufDesc),
  USB_DESCR_TYP_STRING,
  0x46, 0x30, // う
  0x6A, 0x30, // な
  0x4E, 0x30, // ぎ
  0x02, 0x5C, // 専
  0x80, 0x95, // 門
  0x97, 0x5E, // 店
};

unsigned char __code ProdDesc[] = {
  sizeof(ProdDesc),
  USB_DESCR_TYP_STRING,
  0x7A, 0x12, // ቺቻቺቻ
  0x7B, 0x12,
  0x7A, 0x12,
  0x7B, 0x12,
};

unsigned char __code SerialDesc[] = {
  sizeof(SerialDesc),
  USB_DESCR_TYP_STRING,
  0x50, 0x21,
};

__xdata PSTN_LINE_CODING LineCoding = {
  .DTERate0L = 0x00,
  .DTERate0H = 0xE1,   // baud rate 57600
  .DTERate1L = 0x00,
  .DTERate1H = 0x00,
  .bCharFormat = 0x00, // 1 stop bit
  .bParityType = 0x00, // no parity
  .bDataBits = 0x08,   // 8 data bits
};