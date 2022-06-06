/* Teensy LC/3 becomes a USB NS Gamepad

   Select "NS Gamepad" from the "Tools > USB Type" menu
*/
/*
 * MIT License
 * 
 * Copyright (c) 2020 gdsports625@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis. The pullup resistor will interfere with
// the analog voltage.
#include <Bounce2.h>

struct ButtonInput {
    uint8_t pin;      // the digital pin number
    uint8_t control;  // the NSGamepad control
    Bounce  button;   // the Bounce button object
};

#define NUM_INPUTS 18
ButtonInput inputs[NUM_INPUTS] = {
    { 23, NSButton_Y },
    { 22, NSButton_B },
    { 21, NSButton_A },
    { 20, NSButton_X },
    { 7,  NSButton_L },
    { 18, NSButton_R },
    { 6,  NSButton_ZL },
    { 19, NSButton_ZR },
    { 8,  NSButton_Minus },
    { 12, NSButton_Plus },
    { 9,  NSButton_L3 },
    { 13, NSButton_R3 },
    { 11, NSButton_Home },
    { 10, NSButton_Capture },

    { 2,  NSDPad_Up },
    { 3,  NSDPad_Right },
    { 4,  NSDPad_Down },
    { 5,  NSDPad_Left },
};

void setup() {
    // you can print to the Serial1 port while the NSGamepad is active!
    Serial1.begin(115200);
    Serial1.println("NSGamepad setup");

    for (int i = 0; i < NUM_INPUTS; i++) {
        inputs[i].button.attach( inputs[i].pin , INPUT_PULLUP  );  //setup the bounce instance for the current button
        inputs[i].button.interval(10);                             // interval in ms
    }

    // Sends a clean HID report to the host.
    NSGamepad.begin();
}

typedef struct axis_t {
    uint16_t adcMin;
    uint16_t adcMax;
    const uint8_t toMin;
    const uint8_t toMax;
} axis_t;

const uint16_t AxisOffset = 128;

axis_t LeftX = {
    AxisOffset, 1024-AxisOffset, 0, 255
};
axis_t LeftY = {
    AxisOffset, 1024-AxisOffset, 255, 0
};
axis_t RightX = {
    AxisOffset, 1024-AxisOffset, 0, 255
};
axis_t RightY = {
    AxisOffset, 1024-AxisOffset, 255, 0
};

// Dynamically determine the pot limits because of my craptastic
// analog sticks.
uint8_t axisRead(int analogPin, struct axis_t &ax)
{
    uint16_t x = analogRead(analogPin);
    if (x > ax.adcMax) ax.adcMax = x;
    if (x < ax.adcMin) ax.adcMin = x;
    return map(x, ax.adcMin, ax.adcMax, ax.toMin, ax.toMax);
}

void loop() {
    for (int i = 0; i < NUM_INPUTS; i++) {
        // Update the Bounce instance
        inputs[i].button.update();
        
        // Button fell means button pressed
        if ( inputs[i].button.fell() ) {
            NSGamepad.press(inputs[i].control);
        }
        else if ( inputs[i].button.rose() ) {
            NSGamepad.release(inputs[i].control);
        }
    }

    NSGamepad.set(NSAxis_LeftY,  axisRead(3, LeftY));
    NSGamepad.set(NSAxis_LeftX,  axisRead(2, LeftX));
    NSGamepad.set(NSAxis_RightY, axisRead(1, RightY));
    NSGamepad.set(NSAxis_RightX, axisRead(0, RightX));

    NSGamepad.loop();
}
