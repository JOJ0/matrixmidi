#include <MIDI.h>
#include <advancedSerial.h>
#include <Button.h>
#define USBserial Serial
#define VERBOSITY Level::vvv

// mode 0 -> native MIDI bitrate
// mode 1 -> serial bitrate, MIDI over hairless bridge
// mode 2 -> serial bitrate, debugging via serial monitor
const uint8_t mode = 1;
const uint8_t midi_ch = 1;

const uint8_t ledPin = 13;      // LED pin on most Arduino

const uint8_t outPins[] = {6,7,8}; // the rows
const uint8_t inPins[] = {9,10,11,12}; // the columns
uint8_t buttons[4][3] = {
    {101,102,103},
    {104,105,106},
    {107,108,109},
    {110,111,112},
};

MIDI_CREATE_INSTANCE(HardwareSerial, USBserial, MIDI); // with this method port is selectable here

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(outPins[0], OUTPUT); // our output line for col 0 (1,4,7,*)
    pinMode(outPins[1], OUTPUT); // col 1 (2,5,8,0)
    pinMode(outPins[2], OUTPUT); // col 2 (3,6,9,#)
    pinMode(inPins[0], INPUT); // just for clarity, INPUT is the default mode anyway
    pinMode(inPins[1], INPUT);
    pinMode(inPins[2], INPUT);
    pinMode(inPins[3], INPUT);
    digitalWrite(outPins[0], LOW); // initialize cols with LOW
    digitalWrite(outPins[1], LOW);
    digitalWrite(outPins[2], LOW);
    while(!USBserial); // wait until USBserial is accessible
    if (mode == 0)
    {
        USBserial.begin(31250); // MIDI serial rate -> cheap USB MIDI cable hack
        aSerial.off();  // disable debug output
    }
    else if (mode == 1)
    {
        USBserial.begin(9600); // common serial rate -> hairless midi bridge
        aSerial.off();  // disable debug output
    }
    else if (mode == 2)
    {
        USBserial.begin(9600);  // common serial rate -> debugging
        aSerial.setPrinter(USBserial);  // debugging settings
        aSerial.setFilter(VERBOSITY); // debugging settings
        aSerial.v().pln("Matrix MIDI Ctrl ready...");
        aSerial.on();  // enable debug output
    }
}

// saves the buttons current state. 1=pressed, 0=released
uint8_t buttonsPressedNow[4][3] = {
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

// button state is backuped up to this array after each cycle where a button press was detected,
// so we can compare if the state of the button has changed on the next cycle
uint8_t buttonsPressedLast[4][3] = {
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

void sendCCandLog(uint8_t cc_num, uint8_t cc_value, uint8_t _midi_ch, uint8_t _mode)
{
    digitalWrite(ledPin, HIGH);
    aSerial.vvv().p("button CC: ").pln(cc_num);
    aSerial.vvv().pln();
    if (mode != 2)
    {
        delay(1);
        MIDI.sendControlChange(cc_num, cc_value, _midi_ch);
        delay(5);
    }
}

void loop()
{
    for (uint8_t col = 0; col < sizeof(outPins); col++)
    {
        digitalWrite(outPins[col], HIGH);
        for (uint8_t row = 0; row < sizeof(inPins); row++)
        {
            // MOMENTARY PRESS BEHAVIOUR:
            // first check what state the button is in
            if (digitalRead(inPins[row]) == HIGH)
            {
                buttonsPressedNow[row][col] = 1;
            }
            else
            {
                buttonsPressedNow[row][col] = 0;
            }
            // then check if it was just pressed or just released
            // no else: nothing to do when state didn't change
            if (buttonsPressedNow[row][col] > buttonsPressedLast[row][col])
            {
                sendCCandLog(buttons[row][col], 65, midi_ch, mode); // just pressed
                                                                   // -> send sth higher than 64
            }
            else if (buttonsPressedNow[row][col] < buttonsPressedLast[row][col])
            {
                sendCCandLog(buttons[row][col], 0, midi_ch, mode); // just released -> send 0
            }
            // save current button state in ...Last array so we can compare against it on next run
            buttonsPressedLast[row][col] = buttonsPressedNow[row][col];
        }
        digitalWrite(outPins[col], LOW);
    }
    //delay(500); // enable for slower debugging
    digitalWrite(ledPin, LOW);
}
