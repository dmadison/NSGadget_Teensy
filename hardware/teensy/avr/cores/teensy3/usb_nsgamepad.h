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

// For backwards compatibility with the old enum name
#define NSButtons NSInputs

enum NSInputs {
  // Buttons (in bitfield)
  NSButton_Y = 0,
  NSButton_B,
  NSButton_A,
  NSButton_X,
  NSButton_L,
  NSButton_R,
  NSButton_ZL,
  NSButton_ZR,
  NSButton_Minus,
  NSButton_Plus,
  NSButton_L3,
  NSButton_R3,
  NSButton_Home,
  NSButton_Capture,
  NSButton_Reserved1,
  NSButton_Reserved2,

  // Analog Axes
  NSAxis_LeftX,
  NSAxis_LeftY,
  NSAxis_RightX,
  NSAxis_RightY,

  // Directional Pads
  NSDPad_Up,
  NSDPad_Down,
  NSDPad_Left,
  NSDPad_Right,

  // Deprecated Input Names
  NSButton_LeftTrigger = NSButton_L,
  NSButton_RightTrigger = NSButton_R,
  NSButton_LeftThrottle = NSButton_ZL,
  NSButton_RightThrottle = NSButton_ZR,
  NSButton_LeftStick = NSButton_L3,
  NSButton_RightStick = NSButton_R3,
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
        usb_nsgamepad_class();

        void begin(void);
        bool connected(void);
        
        void loop(void);
        void send(void);
        void send(HID_NSGamepadReport_Data_t& r);

        void press(uint8_t b);
        void release(uint8_t b);

        void set(uint8_t input, uint8_t value);
        uint8_t get(uint8_t input) const;

        void releaseAll(void);

        // Deprecated functions
        void end(void) __attribute__((deprecated));
        void write(void) __attribute__((deprecated));
        void write(void* r) __attribute__((deprecated));
        void buttons(uint16_t b) __attribute__((deprecated));
        void leftXAxis(uint8_t a) __attribute__((deprecated));
        void leftYAxis(uint8_t a) __attribute__((deprecated));
        void rightXAxis(uint8_t a) __attribute__((deprecated));
        void rightYAxis(uint8_t a) __attribute__((deprecated));
        void dPad(int8_t d) __attribute__((deprecated));

    protected:
        uint32_t startMillis;
    private:
        HID_NSGamepadReport_Data_t* const report;

        struct ATTRIBUTE_PACKED DPadButtons {
            union {
                uint8_t raw;
                struct {
                    bool up    : 1;
                    bool down  : 1;
                    bool left  : 1;
                    bool right : 1;
                };
            };
        } dpadBuffer;

        static uint8_t encodeDpad(DPadButtons dpad);
        static bool isAxis(uint8_t input);
};
extern usb_nsgamepad_class NSGamepad;

#endif // __cplusplus

#endif // NSGAMEPAD_INTERFACE

#endif // USBnsgamepad_h_
