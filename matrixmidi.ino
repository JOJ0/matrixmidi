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

void loop()
{
    for (uint8_t col = 0; col < sizeof(outPins); col++)
    {
        digitalWrite(outPins[col], HIGH);
        for (uint8_t row = 0; row < sizeof(inPins); row++)
        {
            if (digitalRead(inPins[row]) == HIGH) // blink led if inPin is HIGH)
            {
                digitalWrite(ledPin, HIGH);
                aSerial.vvv().p("yes, it's high: ").p(col).p(" ").pln(row);
                aSerial.vvv().p("button CC: ").pln(buttons[row][col]);
                aSerial.vvv().pln();
                if (mode != 2)
                {
                    delay(5);
                    MIDI.sendControlChange(buttons[row][col], 65, midi_ch);
                    delay(250);
                }
            }
        }
        digitalWrite(outPins[col], LOW);
    }
    //delay(500); // enable for slower debugging
    digitalWrite(ledPin, LOW);
}
