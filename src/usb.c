#include "usb.h"

#include <string.h>
#include "ch552.h"

__xdata __at(0x0000) uint8_t Ep0Buffer[DEFAULT_ENDP0_SIZE];
__xdata __at(0x0008) uint8_t Ep1Buffer[MAX_PACKET_SIZE];
__xdata __at(0x0048) uint8_t Ep2Buffer[2 * MAX_PACKET_SIZE];

#define UsbSetupBuf ((PUSB_SETUP_REQ)Ep0Buffer)

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
  .iManufacturer = 0x01,
  .iProduct = 0x02,
  .iSerialNumber = 0x03,
  .bNumConfigurations = 0x01,
};

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

uint8_t *pDescr;
uint16_t SetupLen;
uint8_t SetupReq, SetAddr, UsbConfig;

void USBDevConfig(void) {
  USB_CTRL = 0x00;
  USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;
  USB_DEV_AD = 0x00;

  USB_CTRL &= ~bUC_LOW_SPEED;
  UDEV_CTRL &= ~bUD_LOW_SPEED;
  UDEV_CTRL = bUD_PD_DIS | bUD_PORT_EN;
}

void USBIntConfig(void) {
  USB_INT_EN |= bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
  USB_INT_FG |= 0x1F;
  IE_USB = 1;
  EA = 1;
}

void USBDevEPConfig(void) {
  UEP0_DMA = (uint16_t)Ep0Buffer;
  UEP1_DMA = (uint16_t)Ep1Buffer;
  UEP2_DMA = (uint16_t)Ep2Buffer;

  UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;

  UEP4_1_MOD = bUEP1_TX_EN;
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;

  UEP2_3_MOD = bUEP2_RX_EN | bUEP2_TX_EN;
  UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
}

void SendData(uint8_t *buf) {
  uint8_t tx_len;
  for (tx_len = 0; buf[tx_len] != 0; tx_len++) {
    Ep2Buffer[MAX_PACKET_SIZE + tx_len] = buf[tx_len];
  }
  UEP2_T_LEN = tx_len;
  UEP2_CTRL &= ~MASK_UEP_T_RES;
}

void HandleGetDescriptor(uint16_t *tx_len) {
  uint8_t descType = UsbSetupBuf->wValueH;
  switch (descType) {
    case USB_DESCR_TYP_DEVICE:
      pDescr = (uint8_t *)&DevDesc;
      *tx_len = sizeof(DevDesc);
      break;
    case USB_DESCR_TYP_CONFIG:
      pDescr = (uint8_t *)&CfgDesc;
      *tx_len = sizeof(CfgDesc);
      break;
    case USB_DESCR_TYP_STRING:
      uint8_t descIndex = UsbSetupBuf->wValueL;
      if (descIndex == 0) {
        pDescr = (uint8_t *)&LangDesc;
        *tx_len = LangDesc[0];
      } else if (descIndex == 1) {
        pDescr = (uint8_t *)&ManufDesc;
        *tx_len = ManufDesc[0];
      } else if (descIndex == 2) {
        pDescr = (uint8_t *)&ProdDesc;
        *tx_len = ProdDesc[0];
      } else if (descIndex == 3) {
        pDescr = (uint8_t *)&SerialDesc;
        *tx_len = SerialDesc[0];
      }
      break;
    default:
      *tx_len = 0xFF;
      break;
  }

  if (SetupLen > *tx_len) {
    SetupLen = *tx_len;
  }
  *tx_len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;
  memcpy(Ep0Buffer, pDescr, *tx_len);
  SetupLen -= *tx_len;
  pDescr += *tx_len;
  return;
}

void SetupEp0(void) {
  uint16_t tx_len = 0x00;
  if (USB_RX_LEN != sizeof(USB_SETUP_REQ)) {
    return;
  }
  SetupReq = UsbSetupBuf->bRequest;
  SetupLen = ((uint16_t)UsbSetupBuf->wLengthH << 8) | (UsbSetupBuf->wLengthL);
  if ((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) == USB_REQ_TYP_STANDARD) {
    // standard request
    switch (SetupReq) {
      case USB_GET_DESCRIPTOR:
        HandleGetDescriptor(&tx_len);
        break;
      case USB_SET_ADDRESS:
        SetupLen = UsbSetupBuf->wValueL;
        break;
      case USB_SET_CONFIGURATION:
        UsbConfig = UsbSetupBuf->wValueL;
        break;
      case USB_GET_CONFIGURATION:
        Ep0Buffer[0] = UsbConfig;
        tx_len = 1;
        break;
      case USB_GET_INTERFACE:
        break;
      case USB_CLEAR_FEATURE:
        break;
      case USB_SET_FEATURE:
        break;
      case USB_GET_STATUS:
        Ep0Buffer[0] = 0x00;
        Ep0Buffer[1] = 0x00;
        tx_len = 2;
        break;
      default:
        tx_len = 0xFF;
        break;
    }
  } else {
    switch (SetupReq) {
      case PSTN_GET_LINE_CODING:
        pDescr = (uint8_t *)&LineCoding;
        tx_len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;
        memcpy(Ep0Buffer, pDescr, tx_len);
        SetupLen -= tx_len;
        pDescr += tx_len;
        break;
      default:
        tx_len = 0xFF;
        break;
    }
  }

  if (tx_len == 0xFF) {
    SetupReq = 0xFF;
    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;
  } else if (tx_len <= DEFAULT_ENDP0_SIZE) {
    UEP0_T_LEN = tx_len;
    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;
  } else {
    UEP0_T_LEN = 0;
    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;
  }
}

void InEp0(void) {
  uint16_t tx_len;
  switch (SetupReq) {
    case USB_GET_DESCRIPTOR:
      tx_len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;
      memcpy(Ep0Buffer, pDescr, tx_len);
      SetupLen -= tx_len;
      pDescr += tx_len;
      UEP0_T_LEN = tx_len;
      UEP0_CTRL ^= bUEP_T_TOG;
      break;
    case USB_SET_ADDRESS:
      USB_DEV_AD = SetupLen;
      UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
      break;
    default:
      UEP0_T_LEN = 0;
      UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
      break;
  }
}

void OutEp0(void) {
  if (SetupReq == PSTN_SET_LINE_CODING) {
    if (!U_TOG_OK) {
      return;
    }
    memcpy((uint8_t *)&LineCoding, UsbSetupBuf, USB_RX_LEN);
    UEP0_T_LEN = 0;
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;
  } else {
    UEP0_T_LEN = 0;
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
  }
}

void USBInterrupt(void) __interrupt(INT_NO_USB) __using(1) {
  if (UIF_TRANSFER) {
    // USB transfer complete
    switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP)) {
      case UIS_TOKEN_SETUP | 0:
        // EP0 SETUP
        SetupEp0();
        break;
      case UIS_TOKEN_IN | 0:
        // EP0 IN
        InEp0();
        break;
      case UIS_TOKEN_OUT | 0:
        // EP0 OUT
        OutEp0();
        break;
      case UIS_TOKEN_IN | 1:
        // EP1 IN
        UEP1_T_LEN = 0;
        UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;
        break;
      case UIS_TOKEN_IN | 2:
        // EP2 IN
        UEP2_T_LEN = 0;
        UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;
        break;
      case UIS_TOKEN_OUT | 2:
        // EP2 OUT
        if (!U_TOG_OK) break;
        UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_R_RES | UEP_R_RES_NAK;
        break;
      default:
        break;
    }
    UIF_TRANSFER = 0;
  }

  if (UIF_BUS_RST) {
    // USB bus reset
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
    UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
    USB_DEV_AD = 0x00;
    UIF_SUSPEND = 0;
    UIF_TRANSFER = 0;
    UIF_BUS_RST = 0;
    UsbConfig = 0;
  }

  if (UIF_SUSPEND) {
    // USB bus suspend/wakeup
    UIF_SUSPEND = 0;
    if (USB_MIS_ST & bUMS_SUSPEND) {
      // suspend
    }
  } else {
    USB_INT_FG = 0x00;
  }
}
