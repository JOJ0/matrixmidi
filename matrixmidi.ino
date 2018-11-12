#include <MIDI.h>
#include <advancedSerial.h>
#define USBserial Serial
#define VERBOSITY Level::vvv
#define DEBUGGING on()
//#define DEBUGGING off()


const uint8_t ledPin = 13;      // LED pin on most Arduino

const uint8_t outPins[] = {6,7,8}; // the rows
const uint8_t inPins[] = {9,10,11,12}; // the columns
const uint8_t buttons[][3] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'},
};

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
    USBserial.begin(115200); // debugging settings start here
    aSerial.setPrinter(USBserial);
    aSerial.setFilter(VERBOSITY);
    aSerial.DEBUGGING;  // enable/disable debug output in #define section
}

void loop()
{
    for (int col = 0; col < sizeof(outPins); col++)
    {
        digitalWrite(outPins[col], HIGH);
        for (int row = 0; row < sizeof(inPins); row++)
        {
            if (digitalRead(inPins[row]) == HIGH) // blink led if inPin is HIGH)
            {
                digitalWrite(ledPin, HIGH);
                aSerial.vvv().p("yes, it's high: ").p(col).p(" ").pln(row);
                aSerial.vvv().pln();
            }
        }
        digitalWrite(outPins[col], LOW);
        aSerial.vvv().pln();
    }
    delay(500); // enable for slower debugging, otherwise lit led can't be seen ;-)
    digitalWrite(ledPin, LOW);
}
