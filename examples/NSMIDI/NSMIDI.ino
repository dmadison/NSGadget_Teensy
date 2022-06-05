/*
 * Select Board: Teensy 3.6
 * Select NS Gamepad on the "Tools > USB Type" menu.
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

#include <USBHost_t36.h>

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);
KeyboardController keyboard1(myusb);

const uint8_t LEFT_STICK_LEFT = 128;
const uint8_t LEFT_STICK_RIGHT = 129;
const uint8_t LEFT_STICK_UP = 130;
const uint8_t LEFT_STICK_DOWN = 131;
const uint8_t RIGHT_STICK_LEFT = 132;
const uint8_t RIGHT_STICK_RIGHT = 133;

const uint8_t NO_INPUT = 255;

const uint8_t NOTE2BUTTON[] = {
    NO_INPUT,          // Note 0
    NO_INPUT,          // Note 1
    NO_INPUT,          // Note 2
    NO_INPUT,          // Note 3
    NO_INPUT,          // Note 4
    NO_INPUT,          // Note 5
    NO_INPUT,          // Note 6
    NO_INPUT,          // Note 7
    NO_INPUT,          // Note 8
    NO_INPUT,          // Note 9
    NO_INPUT,          // Note 10
    NO_INPUT,          // Note 11
    NO_INPUT,          // Note 12
    NO_INPUT,          // Note 13
    NO_INPUT,          // Note 14
    NO_INPUT,          // Note 15
    NO_INPUT,          // Note 16
    NO_INPUT,          // Note 17
    NO_INPUT,          // Note 18
    NO_INPUT,          // Note 19
    NO_INPUT,          // Note 20
    NO_INPUT,          // Note 21
    NO_INPUT,          // Note 22
    NO_INPUT,          // Note 23
    NO_INPUT,          // Note 24
    NO_INPUT,          // Note 25
    NO_INPUT,          // Note 26
    NO_INPUT,          // Note 27
    NO_INPUT,          // Note 28
    NO_INPUT,          // Note 29
    NO_INPUT,          // Note 30
    NO_INPUT,          // Note 31
    NO_INPUT,          // Note 32
    NO_INPUT,          // Note 33
    NO_INPUT,          // Note 34
    NO_INPUT,          // Note 35
    NO_INPUT,          // Note 36
    NO_INPUT,          // Note 37
    NO_INPUT,          // Note 38
    NO_INPUT,          // Note 39
    NO_INPUT,          // Note 40
    NO_INPUT,          // Note 41
    NO_INPUT,          // Note 42
    NO_INPUT,          // Note 43
    NO_INPUT,          // Note 44
    NO_INPUT,          // Note 45
    NO_INPUT,          // Note 46
    NO_INPUT,          // Note 47
    NO_INPUT,          // Note 48
    NO_INPUT,          // Note 49
    NO_INPUT,          // Note 50
    NO_INPUT,          // Note 51
    NO_INPUT,          // Note 52
    NSButton_L,        // W Note 53
    NSButton_Minus,    // B Note 54
    NSDPad_Left,       // W Note 55
    NSButton_Capture,  // B Note 56
    NSDPad_Up,         // W Note 57
    NSButton_ZL,       // B Note 58
    NSDPad_Right,      // W Note 59
    NSDPad_Down,       // W Note 60
    LEFT_STICK_DOWN,   // B Note 61 down
    LEFT_STICK_LEFT,   // W Note 62 left
    LEFT_STICK_UP,     // B Note 63 up
    LEFT_STICK_RIGHT,  // W Note 64 right
    NO_INPUT,          // Note 65
    NO_INPUT,          // Note 66
    NO_INPUT,          // Note 67
    NO_INPUT,          // Note 68
    NO_INPUT,          // Note 69
    NO_INPUT,          // Note 70
    NO_INPUT,          // Note 71
    NO_INPUT,          // Note 72
    NO_INPUT,          // Note 73
    NO_INPUT,          // Note 74
    NO_INPUT,          // Note 75
    NO_INPUT,          // Note 76
    NSButton_B,        // W Note 77
    NSButton_ZR,       // B Note 78
    NSButton_Y,        // W Note 79
    NSButton_Home,     // B Note 80
    NSButton_X,        // W Note 81
    NSButton_Plus,     // B Note 82
    NSButton_A,        // W Note 83
    NSButton_R,        // W Note 84
    NO_INPUT,          // Note 85
    NO_INPUT,          // Note 86
    NO_INPUT,          // Note 87
    NO_INPUT,          // Note 88
    NO_INPUT,          // Note 89
    NO_INPUT,          // Note 90
    NO_INPUT,          // Note 91
    NO_INPUT,          // Note 92
    NO_INPUT,          // Note 93
    NO_INPUT,          // Note 94
    NO_INPUT,          // Note 95
};


// Indexed by value returned by getOemKey().
// See USB HID Usage Table
const uint8_t KEYCODE2BUTTON[] = {
    NO_INPUT,           // 0
    NO_INPUT,           // 1
    NO_INPUT,           // 2
    NO_INPUT,           // 3
    LEFT_STICK_LEFT,    // 4 a
    NO_INPUT,           // 5 b
    NO_INPUT,           // 6 c
    NSButton_X,         // 7 d
    NO_INPUT,           // 8 e
    NSButton_Y,         // 9 f
    NO_INPUT,           // 10 g
    NO_INPUT,           // 11 h
    NO_INPUT,           // 12 i
    NSButton_B,         // 13 j
    NSButton_A,         // 14 k
    RIGHT_STICK_LEFT,   // 15 l
    NO_INPUT,           // 16 m
    NO_INPUT,           // 17 n
    NO_INPUT,           // 18 o
    NO_INPUT,           // 19 p
    NO_INPUT,           // 20 q
    NO_INPUT,           // 21 r
    LEFT_STICK_RIGHT,   // 22 s
    NO_INPUT,           // 23 t
    NO_INPUT,           // 24 u
    NO_INPUT,           // 25 v
    NO_INPUT,           // 26 w
    NO_INPUT,           // 27 x
    NO_INPUT,           // 28 y
    NO_INPUT,           // 29 z
    NSButton_ZL,        // 30 1
    NSButton_L,         // 31 2
    NSButton_L3,        // 32 3
    NSButton_Minus,     // 33 4
    NSButton_Capture,   // 34 5
    NSButton_Home,      // 35 6
    NSButton_Plus,      // 36 7
    NSButton_R3,        // 37 8
    NSButton_R,         // 38 9
    NSButton_ZR,        // 39 0
    NSButton_A,         // 40 ENTER
    NO_INPUT,           // 41 ESCAPE
    NSButton_B,         // 42 BACKSPACE
    NO_INPUT,           // 43 TAB
    NSButton_ZR,        // 44 SPACE
    NO_INPUT,           // 45 - _
    NO_INPUT,           // 46 = +
    NO_INPUT,           // 47 [ {
    NO_INPUT,           // 48 ] }
    NO_INPUT,           // 49 \ |
    NO_INPUT,           // 50 # ~
    RIGHT_STICK_RIGHT,  // 51 ; :
    NO_INPUT,           // 52 ' "
    NO_INPUT,           // 53 ` ~
    NO_INPUT,           // 54 , <
    NO_INPUT,           // 55 . >
    NO_INPUT,           // 56 / ?
    NO_INPUT,           // 57 Caps Lock
    NO_INPUT,           // 58 F1
    NO_INPUT,           // 59 F2
    NO_INPUT,           // 60 F3
    NO_INPUT,           // 61 F4
    NO_INPUT,           // 62 F5
    NO_INPUT,           // 63 F6
    NO_INPUT,           // 64 F7
    NO_INPUT,           // 65 F8
    NO_INPUT,           // 66 F9
    NO_INPUT,           // 67 F10
    NO_INPUT,           // 68 F11
    NO_INPUT,           // 69 F12
    NO_INPUT,           // 70 PrtScr
    NO_INPUT,           // 71 Scoll Lock
    NO_INPUT,           // 72 Pause
    NO_INPUT,           // 73 Insert
    NO_INPUT,           // 74 Home
    NO_INPUT,           // 75 PageUp
    NO_INPUT,           // 76 DELETE
    NO_INPUT,           // 77 End
    NO_INPUT,           // 78 PageDown
    NSDPad_Right,       // 79 RightArrow
    NSDPad_Left,        // 80 LeftArrow
    NSDPad_Down,        // 81 DownArrow
    NSDPad_Up,          // 82 UpArrow
    NO_INPUT,           // 83
    NO_INPUT,           // 84
    NO_INPUT,           // 85
    NO_INPUT,           // 86
    NO_INPUT,           // 87
    NO_INPUT,           // 88
    NO_INPUT,           // 89
    NO_INPUT,           // 90
    NO_INPUT,           // 91
    NO_INPUT,           // 92
    NO_INPUT,           // 93
    NO_INPUT,           // 94
    NO_INPUT,           // 95
    NO_INPUT,           // 96
    NO_INPUT,           // 97
    NO_INPUT,           // 98
    NO_INPUT,           // 99
    NO_INPUT,           // 100
    NO_INPUT,           // 101
    NO_INPUT,           // 102
    NO_INPUT,           // 103
    NO_INPUT,           // 104
    NO_INPUT,           // 105
    NO_INPUT,           // 106
    NO_INPUT,           // 107
    NO_INPUT,           // 108
    NO_INPUT,           // 109
    NO_INPUT,           // 110
    NO_INPUT,           // 111
    NO_INPUT,           // 112
    NO_INPUT,           // 113
    NO_INPUT,           // 114
    NO_INPUT,           // 115
    NO_INPUT,           // 116
    NO_INPUT,           // 117
    NO_INPUT,           // 118
    NO_INPUT,           // 119
    NO_INPUT,           // 120
    NO_INPUT,           // 121
    NO_INPUT,           // 122
    NO_INPUT,           // 123
    NO_INPUT,           // 124
    NO_INPUT,           // 125
    NO_INPUT,           // 126
    NO_INPUT,           // 127
    NO_INPUT,           // 128
    NO_INPUT,           // 129
    NO_INPUT,           // 130
    NO_INPUT,           // 131
    NO_INPUT,           // 132
    NO_INPUT,           // 133
    NO_INPUT,           // 134
    NO_INPUT,           // 135
    NO_INPUT,           // 136
    NO_INPUT,           // 137
    NO_INPUT,           // 138
    NO_INPUT,           // 139
    NO_INPUT,           // 140
    NO_INPUT,           // 141
    NO_INPUT,           // 142
    NO_INPUT,           // 143
    NO_INPUT,           // 144
    NO_INPUT,           // 145
    NO_INPUT,           // 146
    NO_INPUT,           // 147
    NO_INPUT,           // 148
    NO_INPUT,           // 149
    NO_INPUT,           // 150
    NO_INPUT,           // 151
    NO_INPUT,           // 152
    NO_INPUT,           // 153
    NO_INPUT,           // 154
    NO_INPUT,           // 155
    NO_INPUT,           // 156
    NO_INPUT,           // 157
    NO_INPUT,           // 158
    NO_INPUT,           // 159
    NO_INPUT,           // 160
    NO_INPUT,           // 161
    NO_INPUT,           // 162
    NO_INPUT,           // 163
    NO_INPUT,           // 164
    NO_INPUT,           // 165
    NO_INPUT,           // 166
    NO_INPUT,           // 167
    NO_INPUT,           // 168
    NO_INPUT,           // 169
    NO_INPUT,           // 170
    NO_INPUT,           // 171
    NO_INPUT,           // 172
    NO_INPUT,           // 173
    NO_INPUT,           // 174
    NO_INPUT,           // 175
    NO_INPUT,           // 176
    NO_INPUT,           // 177
    NO_INPUT,           // 178
    NO_INPUT,           // 179
    NO_INPUT,           // 180
    NO_INPUT,           // 181
    NO_INPUT,           // 182
    NO_INPUT,           // 183
    NO_INPUT,           // 184
    NO_INPUT,           // 185
    NO_INPUT,           // 186
    NO_INPUT,           // 187
    NO_INPUT,           // 188
    NO_INPUT,           // 189
    NO_INPUT,           // 190
    NO_INPUT,           // 191
    NO_INPUT,           // 192
    NO_INPUT,           // 193
    NO_INPUT,           // 194
    NO_INPUT,           // 195
    NO_INPUT,           // 196
    NO_INPUT,           // 197
    NO_INPUT,           // 198
    NO_INPUT,           // 199
    NO_INPUT,           // 200
    NO_INPUT,           // 201
    NO_INPUT,           // 202
    NO_INPUT,           // 203
    NO_INPUT,           // 204
    NO_INPUT,           // 205
    NO_INPUT,           // 206
    NO_INPUT,           // 207
    NO_INPUT,           // 208
    NO_INPUT,           // 209
    NO_INPUT,           // 210
    NO_INPUT,           // 211
    NO_INPUT,           // 212
    NO_INPUT,           // 213
    NO_INPUT,           // 214
    NO_INPUT,           // 215
    NO_INPUT,           // 216
    NO_INPUT,           // 217
    NO_INPUT,           // 218
    NO_INPUT,           // 219
    NO_INPUT,           // 220
    NO_INPUT,           // 221
    NO_INPUT,           // 222
    NO_INPUT,           // 223
    NO_INPUT,           // 224
    NO_INPUT,           // 225
    NO_INPUT,           // 226
    NO_INPUT,           // 227
    NO_INPUT,           // 228
    NO_INPUT,           // 229
    NO_INPUT,           // 230
    NO_INPUT,           // 231
    NO_INPUT,           // 232
    NO_INPUT,           // 233
    NO_INPUT,           // 234
    NO_INPUT,           // 235
    NO_INPUT,           // 236
    NO_INPUT,           // 237
    NO_INPUT,           // 238
    NO_INPUT,           // 239
    NO_INPUT,           // 240
    NO_INPUT,           // 241
    NO_INPUT,           // 242
    NO_INPUT,           // 243
    NO_INPUT,           // 244
    NO_INPUT,           // 245
    NO_INPUT,           // 246
    NO_INPUT,           // 247
    NO_INPUT,           // 248
    NO_INPUT,           // 249
    NO_INPUT,           // 250
    NO_INPUT,           // 251
    NO_INPUT,           // 252
    NO_INPUT,           // 253
    NO_INPUT,           // 254
    NO_INPUT,           // 255
};

void setup() {
    NSGamepad.begin();

    // Wait 1.5 seconds before turning on USB Host.  If connected USB devices
    // use too much power, Teensy at least completes USB enumeration, which
    // makes isolating the power issue easier.
    delay(1500);
    myusb.begin();

    keyboard1.attachRawPress(myKeyOn);
    keyboard1.attachRawRelease(myKeyOff);
    midi1.setHandleNoteOn(myNoteOn);
    midi1.setHandleNoteOff(myNoteOff);
}

void loop() {
    // The handler functions are called when midi1 reads data.  They
    // will not be called automatically.  You must call midi1.read()
    // regularly from loop() for midi1 to actually read incoming
    // data and run the handler functions as messages arrive.
    myusb.Task();
    midi1.read();
    NSGamepad.loop();
}

uint8_t Dpad_bits;

void buttonPress(uint8_t button)
{
    if (button == NO_INPUT) return;
    switch (button) {
      case LEFT_STICK_DOWN:
        NSGamepad.set(NSAxis_LeftY, 255);
        break;
      case LEFT_STICK_UP:
        NSGamepad.set(NSAxis_LeftY, 0);
        break;
      case LEFT_STICK_LEFT:
        NSGamepad.set(NSAxis_LeftX, 0);
        break;
      case LEFT_STICK_RIGHT:
        NSGamepad.set(NSAxis_LeftX, 255);
        break;
      case RIGHT_STICK_LEFT:
        NSGamepad.set(NSAxis_RightX, 0);
        break;
      case RIGHT_STICK_RIGHT:
        NSGamepad.set(NSAxis_RightX, 255);
        break;
      default:
        NSGamepad.press(button);
    }
}

void buttonRelease(uint8_t button)
{
    if (button == NO_INPUT) return;
    switch (button) {
      case LEFT_STICK_DOWN:
      case LEFT_STICK_UP:
        NSGamepad.set(NSAxis_LeftY, 128);
        break;
      case LEFT_STICK_LEFT:
      case LEFT_STICK_RIGHT:
        NSGamepad.set(NSAxis_LeftX, 128);
        break;
      case RIGHT_STICK_LEFT:
      case RIGHT_STICK_RIGHT:
        NSGamepad.set(NSAxis_RightX, 128);
        break;
      default:
        NSGamepad.release(button);
    }
}

void myKeyOn(uint8_t oemkey)
{
    if (oemkey >= sizeof(KEYCODE2BUTTON)) return;
    buttonPress(KEYCODE2BUTTON[oemkey]);
}

void myKeyOff(uint8_t oemkey)
{
    if (oemkey >= sizeof(KEYCODE2BUTTON)) return;
    buttonRelease(KEYCODE2BUTTON[oemkey]);
}

void myNoteOn(byte channel, byte note, byte velocity)
{
    if (note >= sizeof(NOTE2BUTTON)) return;
    buttonPress(NOTE2BUTTON[note]);
}

void myNoteOff(byte channel, byte note, byte velocity)
{
    if (note >= sizeof(NOTE2BUTTON)) return;
    buttonRelease(NOTE2BUTTON[note]);
}
