#include <MIDI.h>
#include <advancedSerial.h>
#define USBserial Serial
#define VERBOSITY Level::vvv
#define DEBUGGING on()
//#define DEBUGGING off()


const uint8_t ledPin = 13;      // LED pin on most Arduino

const uint8_t outPins[] = {6,7,8}; // the rows
const uint8_t inPins[] = {9,10,11,12}; // the columns
const uint8_t rowChars[][3] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'},
};

uint8_t inVal0 = 0;
uint8_t inVal1 = 0;
uint8_t inVal2 = 0;
uint8_t inVal3 = 0;
uint8_t pinToTest = 0;      // initialize variable for row test blink

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(outPins[0], OUTPUT); // our output line for col 0 (1,4,7,*)
    pinMode(outPins[1], OUTPUT); // col 1 (2,5,8,0)
    pinMode(outPins[2], OUTPUT); // col 2 (3,6,9,#)
    pinMode(inPins[0], INPUT); // just for clarity, INPUT is the default mode anyway
    pinMode(inPins[1], INPUT); // just for clarity, INPUT is the default mode anyway
    pinMode(inPins[2], INPUT); // just for clarity, INPUT is the default mode anyway
    pinMode(inPins[3], INPUT); // just for clarity, INPUT is the default mode anyway
    digitalWrite(outPins[0], LOW); // set HIGH to check col 0
    digitalWrite(outPins[1], LOW); // set HIGH to check col 1
    digitalWrite(outPins[2], HIGH); // set HIGH to check col 2
    USBserial.begin(115200); // debugging settings start here
    aSerial.setPrinter(USBserial);
    aSerial.setFilter(VERBOSITY);
    aSerial.DEBUGGING;  // enable/disable debug output in #define section
}

void loop()
{
    inVal0 = digitalRead(inPins[0]);
    inVal1 = digitalRead(inPins[1]);
    inVal2 = digitalRead(inPins[2]);
    inVal3 = digitalRead(inPins[3]);
    aSerial.vvv().p("inPin 0 value: ").pln(inVal0);
    aSerial.vvv().p("inPin 1 value: ").pln(inVal1);
    aSerial.vvv().p("inPin 2 value: ").pln(inVal2);
    aSerial.vvv().p("inPin 3 value: ").pln(inVal3);
    aSerial.vvv().pln();
    pinToTest = digitalRead(inPins[3]); // which button row should be tested
    if (pinToTest == HIGH) { // blink led if pinToTest on _row_ 0 is pressed)
        digitalWrite(ledPin, HIGH);
    }
    delay(500);
    digitalWrite(ledPin, LOW);
}
