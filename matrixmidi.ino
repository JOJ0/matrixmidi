#include <MIDI.h>
#include <advancedSerial.h>
#define USBserial Serial
//#define MIDIserial Serial1
#define VERBOSITY Level::vvv
#define DEBUGGING on()
//#define DEBUGGING off()

//#MIDI_CREATE_INSTANCE(HardwareSerial, USBserial, MIDI); // port is selectable here

const uint8_t ledPin = 13;      // LED pin on most Arduino

byte outPins[] = {6,7,8}; // the rows
byte inPins[] = {9,10,11,12}; // the columns
byte rowChars[][4] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'},
};

uint8_t inVal0 = 0;
uint8_t inVal1 = 0;
uint8_t inVal2 = 0;
uint8_t inVal3 = 0;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(outPins[0], OUTPUT);
    pinMode(outPins[1], OUTPUT);
    pinMode(outPins[2], OUTPUT);
    pinMode(inPins[0], INPUT);
    pinMode(inPins[1], INPUT);
    pinMode(inPins[2], INPUT);
    pinMode(inPins[3], INPUT);
    //MIDI.begin(4);                      // Launch MIDI and listen to channel 4
    digitalWrite(outPins[0], HIGH); // HIGH to check col 0
    digitalWrite(outPins[1], LOW); // HIGH to check col 1
    digitalWrite(outPins[2], LOW); // HIGH to check col 2
    USBserial.begin(115200); // debugging settings start here
    aSerial.setPrinter(USBserial);
    aSerial.setFilter(VERBOSITY);
    aSerial.DEBUGGING;  // enable/disable debug output in #define section
}

void loop()
{
    //MIDI.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
    //MIDI.sendNoteOff(42, 0, 1);     // Stop the note
    //if (digitalRead(inPins[0]) == HIGH) {
    //    digitalWrite(ledPin, HIGH);

        inVal0 = digitalRead(inPins[0]);
        inVal1 = digitalRead(inPins[1]);
        inVal2 = digitalRead(inPins[2]);
        inVal3 = digitalRead(inPins[3]);
        aSerial.vvv().p("inPin 0 value: ").pln(inVal0);
        aSerial.vvv().p("inPin 1 value: ").pln(inVal1);
        aSerial.vvv().p("inPin 2 value: ").pln(inVal2);
        aSerial.vvv().p("inPin 3 value: ").pln(inVal3);
        aSerial.vvv().pln();
        delay(500);
    //}
    //else {
    //    digitalWrite(ledPin, LOW);
    //}
    //delay(1000);
}
