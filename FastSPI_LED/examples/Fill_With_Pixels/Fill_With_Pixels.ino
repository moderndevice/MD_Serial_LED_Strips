/* Fill_with_Pixels
 Fills an RGB serial strip with a pixel array from alternating ends of the strip
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
#define DELAY_SPEED 30          // larger numbers are slower - in mS, change to suit
const unsigned long COLOR_MAX = NUM_LEDS * 3;      // controls color change speed, change multiplier

#define PIN 4    // change to your data pin 

#define SMOOTH_COLOR 1           // blend between colors

// Somet chipsets wire in a backwards sort of way
struct CRGB { 
    unsigned char r; 
    unsigned char b; 
    unsigned char g; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };

struct CRGB * leds;

// this is an array of rgb pixels to print
// change this in any way you wish - it shouldn't break the sketch
// make sure you use r,g,b triples though
unsigned char rgbArr[]={ 
    200,100,40,  255,120,0,  50,100,170,  50,200,200, 255,80,20 };

int maxPixArray = sizeof(rgbArr) / 3;  

int count;


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

    // populates LED strip with rgbArr pixels from above

    memset(leds, 0, NUM_LEDS * 3);         // turns off all LEDs
    //  FastSPI_LED.show();


    // push data out to LEDs one pixel at a time
    for(int i = 0 ; i < NUM_LEDS; i++ ) {

        leds[i].r = rgbArr[(i % maxPixArray) * 3    ];
        leds[i].g = rgbArr[((i % maxPixArray) * 3) + 1];
        leds[i].b = rgbArr[((i % maxPixArray) * 3) + 2];

        FastSPI_LED.show();
        delay(DELAY_SPEED);
    }

       // write zeros to memory one at a time
       for(int i = 0 ; i < NUM_LEDS; i++ ) {

            leds[i].r = 0;  
            leds[i].g = 0;  
            leds[i].b = 0; 
        
        FastSPI_LED.show();
        delay(DELAY_SPEED);
    }

}




