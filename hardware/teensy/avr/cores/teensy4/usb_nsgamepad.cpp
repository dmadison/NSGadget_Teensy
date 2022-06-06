/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2020 gdsports
 * Copyright (c) 2022 David Madison
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

#include "usb_nsgamepad.h"

#include "core_pins.h" // for millis()
#include <string.h>    // for memcpy


#ifdef NSGAMEPAD_INTERFACE // defined by usb_dev.h -> usb_desc.h

usb_nsgamepad_class::usb_nsgamepad_class()
    : report((HID_NSGamepadReport_Data_t*)(&usb_nsgamepad_data[0]))
{
    releaseAll();  // fill report struct with idle data
}

bool usb_nsgamepad_class::connected(void) {
    return usb_configuration ? 1 : 0;
}

void usb_nsgamepad_class::begin(void) {
    releaseAll();
    usb_nsgamepad_send();
    startMillis = millis();
};

void usb_nsgamepad_class::loop(void) {
    const uint32_t now = millis();
    // 6 for the NSGAMEPAD_INTERVAL (bInterval) of 5, + 1
    if (now - startMillis >= 6) {
        send();
        startMillis = now;
    }
};

void usb_nsgamepad_class::send(void) {
    report->dPad = encodeDpad(dpadBuffer);  // set dpad from buffer
    usb_nsgamepad_send();
}

void usb_nsgamepad_class::send(HID_NSGamepadReport_Data_t& r) {
    memcpy(report, &r, sizeof(HID_NSGamepadReport_Data_t));
    usb_nsgamepad_send();
}

void usb_nsgamepad_class::press(uint8_t input) {
    if (isAxis(input))
        return;  // cannot 'press' an axis
    if (input == NSDPad)
        set(input, NSGAMEPAD_DPAD_CENTERED);  // 'pressing' the entire thing is the same as centering
    else
        set(input, true);
}

void usb_nsgamepad_class::release(uint8_t input) {
    if (isAxis(input))
        set(input, 0x80);  // 'release' axis to center
    if (input == NSDPad)
        set(input, NSGAMEPAD_DPAD_CENTERED);  // 'release' dpad to center
    else
        set(input, false);
}

void usb_nsgamepad_class::set(uint8_t input, uint8_t value) {
    switch (input) {

     // Analog axes
    case(NSAxis_LeftX):
        report->leftXAxis = value;
        break;
    case(NSAxis_LeftY):
        report->leftYAxis = value;
        break;
    case(NSAxis_RightX):
        report->rightXAxis = value;
        break;
    case(NSAxis_RightY):
        report->rightYAxis = value;
        break;

    // Directional pad
    case(NSDPad):
        // Decode raw directional axis into buffer
        dpadBuffer.up    = (value == NSGAMEPAD_DPAD_UP_LEFT)    || (value <= NSGAMEPAD_DPAD_UP_RIGHT);
        dpadBuffer.right = (value >= NSGAMEPAD_DPAD_UP_RIGHT)   && (value <= NSGAMEPAD_DPAD_DOWN_RIGHT);
        dpadBuffer.down  = (value >= NSGAMEPAD_DPAD_DOWN_RIGHT) && (value <= NSGAMEPAD_DPAD_DOWN_LEFT);
        dpadBuffer.left  = (value >= NSGAMEPAD_DPAD_DOWN_LEFT)  && (value <= NSGAMEPAD_DPAD_UP_LEFT);
        break;
    case(NSDPad_Up):
        dpadBuffer.up = value;
        break;
    case(NSDPad_Down):
        dpadBuffer.down = value;
        break;
    case(NSDPad_Left):
        dpadBuffer.left = value;
        break;
    case(NSDPad_Right):
        dpadBuffer.right = value;
        break;

    // Buttons
    default:
        if (value) report->buttons |= (uint16_t)1 << input;
        else report->buttons &= ~((uint16_t)1 << input);
    }
}

uint8_t usb_nsgamepad_class::get(uint8_t input) const {
    uint8_t output;

    switch (input) {

    // Analog axes
    case(NSAxis_LeftX):
        output = report->leftXAxis;
        break;
    case(NSAxis_LeftY):
        output = report->leftYAxis;
        break;
    case(NSAxis_RightX):
        output = report->rightXAxis;
        break;
    case(NSAxis_RightY):
        output = report->rightYAxis;
        break;

    // Directional pad
    case(NSDPad):
        output = encodeDpad(dpadBuffer);
        break;
    case(NSDPad_Up):
        output = dpadBuffer.up;
        break;
    case(NSDPad_Down):
        output = dpadBuffer.down;
        break;
    case(NSDPad_Left):
        output = dpadBuffer.left;
        break;
    case(NSDPad_Right):
        output = dpadBuffer.right;
        break;

    // Buttons
    default:
        output = report->buttons & (uint16_t)1 << input;
    }

    return output;
}

void usb_nsgamepad_class::releaseAll(void) {
    // release all buttons and center all axes
    memset(report, 0x00, NSGAMEPAD_REPORT_SIZE);
    report->leftXAxis = report->leftYAxis = 0x80;
    report->rightXAxis = report->rightYAxis = 0x80;
    report->dPad = NSGAMEPAD_DPAD_CENTERED;
    dpadBuffer.raw = 0x00;
};

uint8_t usb_nsgamepad_class::encodeDpad(DPadButtons dpad) {
    // Simultaneous Opposite Cardinal Directions (SOCD) cleaner
    // up + down is neutral, left + right is neutral
    if (dpad.up && dpad.down) {
        dpad.up = false;
        dpad.down = false;
    }
    if (dpad.left && dpad.right) {
        dpad.left = false;
        dpad.right = false;
    }

    uint8_t output;
    if (dpad.up) {
             if (dpad.left)  output = NSGAMEPAD_DPAD_UP_LEFT;
        else if (dpad.right) output = NSGAMEPAD_DPAD_UP_RIGHT;
        else                 output = NSGAMEPAD_DPAD_UP;
    }
    else if (dpad.right) {
        if (dpad.down) output = NSGAMEPAD_DPAD_DOWN_RIGHT;
        else           output = NSGAMEPAD_DPAD_RIGHT;
    }
    else if (dpad.down) {
        if (dpad.left) output = NSGAMEPAD_DPAD_DOWN_LEFT;
        else           output = NSGAMEPAD_DPAD_DOWN;
    }
    else if (dpad.left) {
        output = NSGAMEPAD_DPAD_LEFT;
    }
    else {
        output = NSGAMEPAD_DPAD_CENTERED;
    }
    return output;
}

bool usb_nsgamepad_class::isAxis(uint8_t input) {
    switch (input) {
    case(NSAxis_LeftX):
    case(NSAxis_LeftY):
    case(NSAxis_RightX):
    case(NSAxis_RightY):
        return true;
        break;
    default:
        break;
    }
    return false;
}


// Deprecated functions (original API)
void usb_nsgamepad_class::end(void)  {
    releaseAll();
    usb_nsgamepad_send();
};

void usb_nsgamepad_class::write(void)  {
    usb_nsgamepad_send();
};

void usb_nsgamepad_class::write(void* r)  {
    memcpy(report, r, NSGAMEPAD_REPORT_SIZE);
    usb_nsgamepad_send();
};

void usb_nsgamepad_class::buttons(uint16_t b)  {
    report->buttons = b;
};

void usb_nsgamepad_class::leftXAxis(uint8_t a)  {
    report->leftXAxis = a;
};

void usb_nsgamepad_class::leftYAxis(uint8_t a)  {
    report->leftYAxis = a;
};

void usb_nsgamepad_class::rightXAxis(uint8_t a)  {
    report->rightXAxis = a;
};

void usb_nsgamepad_class::rightYAxis(uint8_t a)  {
    report->rightYAxis = a;
};

void usb_nsgamepad_class::dPad(int8_t d)  {
    report->dPad = d;
};

#endif // NSGAMEPAD_INTERFACE
