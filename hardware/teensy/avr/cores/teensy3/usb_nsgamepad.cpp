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
    usb_nsgamepad_send();
}

void usb_nsgamepad_class::send(HID_NSGamepadReport_Data_t& r) {
    memcpy(report, &r, sizeof(HID_NSGamepadReport_Data_t));
    usb_nsgamepad_send();
}

void usb_nsgamepad_class::press(uint8_t b) {
    report->buttons |= (uint16_t)1 << b;
};

void usb_nsgamepad_class::release(uint8_t b) {
    report->buttons &= ~((uint16_t)1 << b);
};

void usb_nsgamepad_class::releaseAll(void) {
    // release all buttons and center all axes
    memset(report, 0x00, NSGAMEPAD_REPORT_SIZE);
    report->leftXAxis = report->leftYAxis = 0x80;
    report->rightXAxis = report->rightYAxis = 0x80;
    report->dPad = NSGAMEPAD_DPAD_CENTERED;
};

void usb_nsgamepad_class::setButton(uint8_t b, bool state) {
    state ? press(b) : release(b);
}

bool usb_nsgamepad_class::getButton(uint8_t b) {
    return report->buttons & ((uint16_t)1 << b);
}

void usb_nsgamepad_class::setAxis(uint8_t a, uint8_t pos) {
    switch (a) {
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

uint8_t usb_nsgamepad_class::getAxis(uint8_t a) {
    uint8_t out;
    switch (a) {
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

bool usb_nsgamepad_class::getDpad(uint8_t d) {
    bool output;

    switch (d) {
    case(NSGAMEPAD_DPAD_UP):
        output = (
            report->dPad == NSGAMEPAD_DPAD_UP
            || report->dPad == NSGAMEPAD_DPAD_UP_LEFT
            || report->dPad == NSGAMEPAD_DPAD_UP_RIGHT
            );
        break;
    case(NSGAMEPAD_DPAD_DOWN):
        output = (
            report->dPad == NSGAMEPAD_DPAD_DOWN
            || report->dPad == NSGAMEPAD_DPAD_DOWN_LEFT
            || report->dPad == NSGAMEPAD_DPAD_DOWN_RIGHT
            );
        break;
    case(NSGAMEPAD_DPAD_RIGHT):
        output = (
            report->dPad == NSGAMEPAD_DPAD_RIGHT
            || report->dPad == NSGAMEPAD_DPAD_UP_RIGHT
            || report->dPad == NSGAMEPAD_DPAD_DOWN_RIGHT
            );
        break;
    case(NSGAMEPAD_DPAD_LEFT):
        output = (
            report->dPad == NSGAMEPAD_DPAD_LEFT
            || report->dPad == NSGAMEPAD_DPAD_UP_LEFT
            || report->dPad == NSGAMEPAD_DPAD_DOWN_LEFT
            );
        break;
    default:
        output = (d == report->dPad);
    }
    return output;
}

void usb_nsgamepad_class::setDpad(bool up, bool down, bool left, bool right) {
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
        if (left)  output = NSGAMEPAD_DPAD_UP_LEFT;
        else if (right) output = NSGAMEPAD_DPAD_UP_RIGHT;
        else            output = NSGAMEPAD_DPAD_UP;
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
        output = NSGAMEPAD_DPAD_LEFT;
    }
    else {
        output = NSGAMEPAD_DPAD_CENTERED;
    }
    report->dPad = output;
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
