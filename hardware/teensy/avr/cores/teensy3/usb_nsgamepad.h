/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/*
 * This file started life as usb_joystick.h then was modified for NSGamepad.
 */

#ifndef USBnsgamepad_h_
#define USBnsgamepad_h_

#include "usb_desc.h"

#if defined(NSGAMEPAD_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_nsgamepad_send(void);
extern uint32_t usb_nsgamepad_data[(NSGAMEPAD_REPORT_SIZE+3)/4];
extern volatile uint8_t usb_configuration;
#ifdef __cplusplus
}
#endif

// Dpad directions
#define NSGAMEPAD_DPAD_CENTERED 0xF
#define NSGAMEPAD_DPAD_UP 0
#define NSGAMEPAD_DPAD_UP_RIGHT 1
#define NSGAMEPAD_DPAD_RIGHT 2
#define NSGAMEPAD_DPAD_DOWN_RIGHT 3
#define NSGAMEPAD_DPAD_DOWN 4
#define NSGAMEPAD_DPAD_DOWN_LEFT 5
#define NSGAMEPAD_DPAD_LEFT 6
#define NSGAMEPAD_DPAD_UP_LEFT 7

#define ATTRIBUTE_PACKED  __attribute__((packed, aligned(1)))

enum NSButtons {
  NSButton_Y = 0,
  NSButton_B,
  NSButton_A,
  NSButton_X,
  NSButton_LeftTrigger,
  NSButton_RightTrigger,
  NSButton_LeftThrottle,
  NSButton_RightThrottle,
  NSButton_Minus,
  NSButton_Plus,
  NSButton_LeftStick,
  NSButton_RightStick,
  NSButton_Home,
  NSButton_Capture,
  NSButton_Reserved1,
  NSButton_Reserved2
};

enum NSAxes {
  // using unique IDs from NSButtons, with separate type
  NSAxis_LeftX = NSButton_Reserved2 + 1,
  NSAxis_LeftY,
  NSAxis_RightX,
  NSAxis_RightY,
};

// 14 Buttons, 4 Axes, 1 D-Pad
typedef struct ATTRIBUTE_PACKED {
    uint16_t buttons;
    uint8_t	dPad;
    uint8_t	leftXAxis;
    uint8_t	leftYAxis;
    uint8_t	rightXAxis;
    uint8_t	rightYAxis;
    uint8_t filler;
} HID_NSGamepadReport_Data_t;

// C++ interface
#ifdef __cplusplus
class usb_nsgamepad_class
{
    public:
        usb_nsgamepad_class()
            : report((HID_NSGamepadReport_Data_t*)(&usb_nsgamepad_data[0]))
        {
            releaseAll();  // fill report struct with idle data
        }
        bool connected(void) {
            return usb_configuration ? 1 : 0;
        }
        void begin(void) {
            end();
            startMillis = millis();
        };
        void end(void) {
            releaseAll();
            usb_nsgamepad_send();
        };
        void loop(void){
            uint32_t endMillis = millis();
            uint32_t deltaMillis;
            if (endMillis >= startMillis) {
                deltaMillis = (endMillis - startMillis);
            }
            else {
                deltaMillis = ((0xFFFFFFFFUL - startMillis) + endMillis);
            }
            if (deltaMillis >= 1) {
                write();
                startMillis = millis();
            }
        };
        void write(void) {
            usb_nsgamepad_send();
        };
        void write(void *r) {
            memcpy(report, r, NSGAMEPAD_REPORT_SIZE);
            usb_nsgamepad_send();
        };
        void press(uint8_t b) {
            report->buttons |= (uint16_t)1 << b;
        };
        void release(uint8_t b) {
            report->buttons &= ~((uint16_t)1 << b);
        };
        void releaseAll(void) {
            // release all buttons and center all axes
            memset(report, 0x00, NSGAMEPAD_REPORT_SIZE);
            report->leftXAxis = report->leftYAxis = 0x80;
            report->rightXAxis = report->rightYAxis = 0x80;
            report->dPad = NSGAMEPAD_DPAD_CENTERED;
        };
        void buttons(uint16_t b) {
            report->buttons = b;
        };
        void setButton(uint8_t b, bool state) {
            state ? press(b) : release(b);
        }
        bool getButton(uint8_t b) {
            return report->buttons & ((uint16_t)1 << b);
        }
        void setAxis(uint8_t a, uint8_t pos) {
            switch(a) {
            case(NSAxis_LeftX):
                report->leftXAxis = pos;
                break;
            case(NSAxis_LeftY):
                report->leftYAxis = pos;
                break;
            case(NSAxis_RightX):
                report->rightXAxis = pos;
                break;
            case(NSAxis_RightY):
                report->rightYAxis = pos;
                break;
            default:
                break;
            }
        }
        uint8_t getAxis(uint8_t a) {
            uint8_t out;
            switch(a) {
            case(NSAxis_LeftX):
                out = report->leftXAxis;
                break;
            case(NSAxis_LeftY):
                out = report->leftYAxis;
                break;
            case(NSAxis_RightX):
                out = report->rightXAxis;
                break;
            case(NSAxis_RightY):
                out = report->rightYAxis;
                break;
            default:
                out = 0;
                break;
            }
            return out;
        }
        void leftXAxis(uint8_t a) {
            report->leftXAxis = a;
        };
        void leftYAxis(uint8_t a) {
            report->leftYAxis = a;
        };
        void rightXAxis(uint8_t a) {
            report->rightXAxis = a;
        };
        void rightYAxis(uint8_t a) {
            report->rightYAxis = a;
        };
        void dPad(int8_t d) {
            report->dPad = d;
        };
        void setDpad(bool up, bool down, bool left, bool right) {
            // Simultaneous Opposite Cardinal Directions (SOCD) cleaner
            // up + down is up only, left + right is neutral
            if (up && down) {
                down = false;
            }
            if (left && right) {
                left = false;
                right = false;
            }

            uint8_t output;
            if (up) {
                if (right) output = NSGAMEPAD_DPAD_UP_RIGHT;
                else       output = NSGAMEPAD_DPAD_UP;
            }
            else if (right) {
                if (down) output = NSGAMEPAD_DPAD_DOWN_RIGHT;
                else      output = NSGAMEPAD_DPAD_RIGHT;
            }
            else if (down) {
                if (left) output = NSGAMEPAD_DPAD_DOWN_LEFT;
                else      output = NSGAMEPAD_DPAD_DOWN;
            }
            else if (left) {
                if (up) output = NSGAMEPAD_DPAD_UP_LEFT;
                else    output = NSGAMEPAD_DPAD_LEFT;
            }
            else {
                output = NSGAMEPAD_DPAD_CENTERED;
            }
            report->dPad = output;
        }
    protected:
        uint32_t startMillis;
    private:
        HID_NSGamepadReport_Data_t* const report;
};
extern usb_nsgamepad_class NSGamepad;

#endif // __cplusplus

#endif // NSGAMEPAD_INTERFACE

#endif // USBnsgamepad_h_
