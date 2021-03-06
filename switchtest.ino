#include <USBComposite.h>

#define SWITCH_CONTROLLER_VENDOR_ID  0x0F0D
#define SWITCH_CONTROLLER_PRODUCT_ID 0x00c1 // HORIPAD

const uint8_t reportDescription[] = {
  0x05, 0x01,           /*  Usage Page (Generic Desktop) */ \
  
  0x09, 0x05,           /*  Usage (Joystick) */ \
  0xA1, 0x01,           /*  Collection (Application) */ \
  /* buttons */ \
  0x15, 0x00,           /*  Logical Minimum (0) */ \
  0x25, 0x01,           /*  Logical Maximum (1) */ \
  0x35, 0x00,           /*  Physical Minimum (0) */ \
  0x45, 0x01,           /*  Physical Maximum (1) */ \
  0x75, 0x01,           /*  Report Size (1) */ \
  0x95, 0x0e,           /*  Report Count (16) */ \
  0x05, 0x09,           /*  Usage Page (9) */ \
  0x19, 0x01,           /*  Usage Minimum (1) */ \
  0x29, 0x0e,           /*  Usage Maximum (16) */ \
  0x81, 0x02,           /*  Input (Data,Var,Abs) */ \
  0x95, 0x02,
  0x81, 0x01,
  /* HAT Switch */ \
  0x05, 0x01,           /*  Usage Page (1) */ \
  0x25, 0x07,           /*  Logical Maximum (7) */ \
  0x46, 315&0xFF,315>>8, /*  Physical Maximum (315) */ \
  0x75, 0x04,           /*  Report Size (4) */ \
  0x95, 0x01,           /*  Report Count (1) */ \
  0x65, 20,             /*  Unit (20) */ \
  0x09, 57,             /*  Usage (57) */ \
  0x81, 66,             /*  Input (66) */ \
    /* additional nibble here for Switch Pro Controller? */ \
  0x65, 0,              /*  Unit (0) */ \
  0x95, 0x01,           /*  Report Count (1) */ \
  0x81, 1,              /*  Input (1) */ \
    /* sticks */ \
  0x26, 255 & 0xFF, 255 >> 8,    /*  Logical Maximum (255) */ \
  0x46, 255 & 0xFF, 255 >> 8,    /*  Physical Maximum (255) */ \
  0x09, 48,             /*  Usage (48) */ \
  0x09, 49,             /*  Usage (49) */ \
  0x09, 50,             /*  Usage (50) */ \
  0x09, 53,             /*  Usage (53) */ \
  0x75, 0x08,           /*  Report Size (8) */ \
  0x95, 0x04,           /*  Report Count (4) */ 
  0x81, 0x02,           /*  Input (Data,Var,Abs) */ 
    /* vendor specific */ 
  0x75, 0x08,           /*  Report Size (8) */ 
  0x95, 0x01,           /*  Report Count (1) */ 
  0x81, 0x01,           /*  Input (1) */ 
  0xC0,                 /*  End Collection */
};

typedef struct {
  uint8_t reportID;
  uint16_t buttons;
  uint8_t dpad; /* D-pad state */
  uint8_t leftX;
  uint8_t leftY;
  uint8_t rightX;
  uint8_t rightY;
  uint8_t pad;
} __packed SwitchControllerReport_t;

class HIDSwitchController : public HIDReporter {
protected:
  SwitchControllerReport_t report; 
public:
  enum {
      DPAD_TOP = 0,
      DPAD_TOPRIGHT = 1,
      DPAD_RIGHT = 2,
      DPAD_BOTRIGHT = 3,
      DPAD_BOTTOM = 4,
      DPAD_BOTLEFT = 5,
      DPAD_LEFT = 6,
      DPAD_TOPLEFT = 7,
      DPAD_NEUTRAL = 8,
  };

    // YBAXLR Zl Zr - + Lc Rc HOME CAP
  enum {
      BUTTON_Y = 0,
      BUTTON_B = 1,
      BUTTON_A = 2,
      BUTTON_X = 3,
      BUTTON_L = 4,
      BUTTON_R = 5,
      BUTTON_ZL = 6,
      BUTTON_ZR = 7,
      BUTTON_MINUS = 8,
      BUTTON_PLUS = 9,
      BUTTON_LEFT_CLICK = 10,
      BUTTON_RIGHT_CLICK = 11,
      BUTTON_HOME = 12,
      BUTTON_CAPTURE = 13,
      BUTTON_EXTRA1 = 14,
      BUTTON_EXTRA2 = 15,
  };

  void button(uint8_t b, bool val) {
    uint16_t mask = ((uint16_t)1 << b);

    if (val) {
          report.buttons |= mask;
    } else {
          report.buttons &= ~mask;
    }
  }

  void dpad(uint8_t value) {
    report.dpad = value;
  }

  void X(uint8_t v) {
    report.leftX = v;
  }
  
  void Y(uint8_t v) {
    report.leftY = v;
  }
  
  void rightX(uint8_t v) {
    report.rightX = v;
  }
  
  void rightY(uint8_t v) {
    report.rightY = v;
  }

  HIDSwitchController(USBHID& HID) 
            : HIDReporter(HID, NULL, (uint8_t*)&report, sizeof(report), 0) {
        report.reportID = 0; 
        report.buttons = 0;
        report.dpad = DPAD_NEUTRAL;
        report.leftX = 128;
        report.leftY = 128;
        report.rightX = 128;
        report.rightY = 128;
        report.pad = 0;
    }
};

USBHID HID;
HIDSwitchController controller(HID);

void setup() {
  USBComposite.setVendorId(SWITCH_CONTROLLER_VENDOR_ID);
  USBComposite.setProductId(SWITCH_CONTROLLER_PRODUCT_ID);
  USBComposite.setProductString("HORIPAD S");
  USBComposite.setManufacturerString("Omega Centauri Software");
  HID.setReportDescriptor(reportDescription, sizeof(reportDescription));
  HID.registerComponent();
  USBComposite.begin();  
  while (!USBComposite);
  delay(1000);
}

void loop() {
  controller.X(16);
  controller.sendReport();
  delay(300);
  controller.X(255-16);
  controller.sendReport();
  delay(300);
  controller.X(128);
  controller.sendReport();
  delay(300);
  /*
   for (int i=0; i<=0; i++) {
    controller.button(i,true);
    controller.sendReport();
    delay(100);
    controller.button(i,false);
    controller.sendReport();
    delay(100);
  } */
}
