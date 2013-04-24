/* one_pixel_forward_back
 Sends one pixel to the end of a five meter strip of RGB LED's,
 and then back again.
 http://shop.moderndevice.com/products/rgb-led-strips-ucs1903
 code in the  public domain. ModernDevice.com Paul Badger  
 
  For use with Modern Device Serial LED strips http://shop.moderndevice.com/products/rgb-led-strips-ucs1903
 This code is in the  public domain. moderndevice.com  Paul Badger 2013
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
#define DELAY_SPEED 25  // larger numbers are slower - in mS change to suit

// The folowing struct is definitely correct for the Modern Device strips
// There must be some chipsets with blue and green swapped out there
struct CRGB { 
    unsigned char r; 
    unsigned char b; 
    unsigned char g; 
};

struct CRGB *leds; //pointer to the beginning of the struct

#define PIN 4    // change to your data pin

// This is an array of rgb pixels to display. Feel free to add, delete, 
// or change this anyway you wish but be sure to add or delete values in r,g,b triplets.
// One triplet represents a pixel to display
unsigned char pixelArr[]={ 
    40,40,40,   255,170,0,  255,60,20,  0,128,128,  200,0,200   };

char temp[3];    // one pixel buff

int maxPixArray = sizeof(pixelArr) / 3;   // lets sketches know where to wrap around the pixelArray

void setup()
{
    FastSPI_LED.setLeds(NUM_LEDS);
    FastSPI_LED.setChipset(CFastSPI_LED::SPI_UCS1903);
    FastSPI_LED.setPin(PIN);
    FastSPI_LED.init();
    FastSPI_LED.start();

    leds = (struct CRGB*)FastSPI_LED.getRGBData();   // set leds pointing at the buffer
}

void loop() { 
    //index through the Pixel Array of colors  new pixel color each time
    for(int i = 0 ; i < maxPixArray ; i++ ) { 

        temp[0] = pixelArr[i * 3];            // r
        temp[1] = pixelArr[(i * 3) + 1];      // g
        temp[2] = pixelArr[(i * 3) + 2];      // b 

        memset(leds, 0, NUM_LEDS * 3);        // turns off all LEDs - try commenting this out
        for (int j = 0; j < NUM_LEDS; j++ ) {
            // Serial.println(j);
            memset(leds, 0, NUM_LEDS * 3);  // turns off all LEDs - try commenting this out
            leds[j].r = temp[0];
            leds[j].g = temp[1];
            leds[j].b = temp[2];
            FastSPI_LED.show();               // turn them back on
            delay(DELAY_SPEED);
        }

        memset(leds, 0, NUM_LEDS * 3);    // turns off all LEDs

        for (int j = NUM_LEDS - 1; j >= 0 ; j-- ) {
            // Serial.println(j);
            memset(leds, 0, NUM_LEDS * 3);  // turns off all LEDs - try commenting this out
            leds[j].r = temp[0];
            leds[j].g = temp[1];
            leds[j].b = temp[2];
            FastSPI_LED.show();              // display the beauty
            delay(DELAY_SPEED);
        }
    }
}






