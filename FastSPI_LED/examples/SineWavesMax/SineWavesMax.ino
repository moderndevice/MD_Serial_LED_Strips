/* SineWaves
 Uses three sine waves to create a complex pattern.  This is the same as the other Sine sketch with higher key settings. 
 Example Code for use with Modern Device Serial LED strips http://shop.moderndevice.com/products/rgb-led-strips-ucs1903
 This code is in the  public domain. moderndevice.com  Paul Badger 4-2013
 The FastSPI_LED library carries an MIT license. http://code.google.com/p/fastspi/
 
 Hardware Setup:
 2 connectiions to your Arduino: Ground (white wire on orignal stip header - use black if you are soldering on wires) 
 to Arduino Ground and Data (blue wire) to data pin (see "PIN" below).
 2 connections from 12 volt power supply to the strip Ground (black wire) and +12 volts (red wire)
 to the stip.
 */

#include <FastSPI_LED.h>

// There are 30 LEDs per meter on the MD strips but 3 LEDs are wired in series, so there are
// 10 LEDs (pixels really) per meter. Set "#define NUM_LEDS 50" to however many meters you have
// times 10. Or count the number of chips on your section of strip.

#define NUM_LEDS 50
#define DELAY_SPEED 30     // larger numbers are slower - in mS, change to suit
const unsigned int COLOR_MAX = NUM_LEDS * 3;      // controls color change speed, change multiplier

// x_COLOR_DEPTH sets the variation in one color, smaller values = less variation, values
// greater than 128 will clip (go to 0 or 256 for longer than "normal"). Experiment!
const  int R_COLOR_DEPTH=256;       // valid values btwn 1 and 128   1 = pastel white, 128 is brightly colored 
const  int G_COLOR_DEPTH=128;
const  int B_COLOR_DEPTH=150;

// in most circumstances you probably want OFFSET to be about the same value as COLOR_DEPTH, 
// but changing the offset a bit can help add or subtract a particular primary from the changing mix.

const  int R_OFFSET = 128;
const  int  G_OFFSET = 100;
const  int  B_OFFSET = 64;


#define PIN 4    // change to your data pin 

float sinValX, sinValY, sinValZ, X, Y, Z;    // floats for sine waves

// This is correct for the Modern Device RGB serial strips
// Some chipsets must be backwards

struct CRGB { 
    unsigned char r; 
    unsigned char b; 
    unsigned char g; 
};

// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB * leds;

void setup()
{
    Serial.begin(9600);
    FastSPI_LED.setLeds(NUM_LEDS);
    FastSPI_LED.setChipset(CFastSPI_LED::SPI_UCS1903);
    FastSPI_LED.setPin(PIN);
    FastSPI_LED.init();
    FastSPI_LED.start();
    leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
    Serial.println("Start"); 
}

void loop() { 

    // increment variables - experiment with values!
    // these values control the speed of color change
    // very small numbers could be useful (eg .002) coiuld be useful in creating really subtly changing color schemes
    sinValX  += .03;
    sinValY  += .045;
    sinValZ  += .025;

    memset(leds, 0, NUM_LEDS * 3);         // turns off all LEDs
    // push data out to LEDs one pixel at a time
    for(int i = 0 ; i < NUM_LEDS; i++ ) {

        // sin() will return a float between -1 
        // just scale to appropriate levels
        // (i * .3) is the color change per LED change in the string.
        // Increase the constant for more quickly changing pixels.
        //  and vice versa. This multiplier
        // can also be zero for strings of all the same color. Experiment!

        X =  (sin(sinValX + (i * .07)) * R_COLOR_DEPTH) + R_OFFSET;
        X = constrain(X, 0, 255);
        Y= (sin(sinValY + (i * .07)) * G_COLOR_DEPTH) + G_OFFSET; 
        Y = constrain(Y, 0, 255);
        Z =  (sin(sinValZ + (i * .061)) * B_COLOR_DEPTH) + B_OFFSET; 
        Z = constrain(Z, 0, 255);

        leds[i].r =  X;
        Serial.println(Z);
        leds[i].g = Y;
        leds[i].b = Z;

    }
    FastSPI_LED.show();
    delay(20);
}













