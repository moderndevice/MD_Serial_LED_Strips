/* marquee_forward_back
 This sketch implements a classic movie marquee marching lights - (marching ants on a computer monitor),
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
#define DELAY_SPEED 100          // speed of scroll. Larger numbers are slower - in mS, change to suit.
const int MARCHING_ANTS = 7;     //  interval between lights that are on. Set to 1 for solid on.
const unsigned long COLOR_MAX = NUM_LEDS * 3;      // controls color change speed, change multiplier only.

#define PIN 4    // change to your data pin 

#define SMOOTH_COLOR 1           // blend between colors

// This is an array of rgb pixels to display. Feel free to add, delete, 
// or change this anyway as you wish, but be sure to add or delete values 
// in r,g,b triplets. One triplet represents one color to display.
// Requires at least two triplet entries. To stop the color change you can just
// the two entries to the same color.
unsigned char pixelArr[]={ 
    40,40,40,   255,170,0,  255,60,20,  50,128,128,  200,0,200  };
int maxPixArray = sizeof(pixelArr) / 3;   // lets sketches know where to wrap around the pixelArray

// The folowing struct is definitely correct for the Modern Device strips
// There must be some chipsets with blue and green swapped out there
struct CRGB { 
    unsigned char r; 
    unsigned char b; 
    unsigned char g; 
};

struct CRGB *leds;  //pointer to the beginning of the struct
byte temp[6];       // two pixel buffer

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
    for (int k = 0 ; k < maxPixArray ; k++ ) { 

        temp[0] = pixelArr[k * 3];            // r
        temp[1] = pixelArr[(k * 3) + 1];      // g
        temp[2] = pixelArr[(k * 3) + 2];      // b

        //memset(leds, 0, NUM_LEDS * 3);        // turns off all LEDs - try commenting this out

        // The first "for" loop controls how fast the colors change. To stop the color change you can just
        // delete or comment out all the entries in the pixelArr except two. The loop probably should be
        // a multiple of NUM_LEDS to change seamlessly.

        for (unsigned long i = 1; i < COLOR_MAX; i++ ){  

#ifdef SMOOTH_COLOR
            // This is slightly ugly. The big picture of the idea is to get the next pixel
            // "k + 1" and average between the two pixels as determined by the for loop "i" value

            temp[0] = pixelArr[k * 3];              // r
            temp[1] = pixelArr[(k * 3) + 1];        // g
            temp[2] = pixelArr[(k * 3) + 2];        // b

            temp[3] = pixelArr[((k + 1) % maxPixArray) * 3];          // r
            temp[4] = pixelArr[(((k + 1) % maxPixArray) * 3) + 1 ];   // g
            temp[5] = pixelArr[(((k + 1) % maxPixArray) * 3) + 2 ];   // b

            temp[0] = (temp[0] * (COLOR_MAX - i ) + (temp[3] * i )) / COLOR_MAX;    // r
            temp[1] = (temp[1] * (COLOR_MAX - i ) + (temp[4] * i )) / COLOR_MAX;    // g
            temp[2] = (temp[2] * (COLOR_MAX - i ) + (temp[5] * i )) / COLOR_MAX;    // b    
#endif         

            for (int j = 0; j < NUM_LEDS; j++ ){
                // Serial.println(j);

                if (((j % MARCHING_ANTS) + (i % MARCHING_ANTS)) == ( MARCHING_ANTS - 1)) {
                    leds[j].r = temp[0];
                    leds[j].g = temp[1];
                    leds[j].b = temp[2];
                }
            }
            FastSPI_LED.show();               // turn them back on
            delay(DELAY_SPEED);
            memset(leds, 0, NUM_LEDS * 3);    // turns off all LEDs - try commenting this out
        }

        //  reverse direction of the march, comment in this section to see the 
        //  "crawl" direction reverse.
        /* 
         for (int i =  NUM_LEDS - 1; i >= 1  ; i-- ){
         
         for (int j = NUM_LEDS - 1; j >=0 ; j-- ){
         // Serial.println(j);
         
         if (((j % MARCHING_ANTS) + (i % MARCHING_ANTS)) == ( MARCHING_ANTS - 1)) {
         leds[j].r = temp[0];
         leds[j].g = temp[1];
         leds[j].b = temp[2];
         }
         }
         FastSPI_LED.show();               // turn them back on
         delay(DELAY_SPEED);
         memset(leds, 0, NUM_LEDS * 3);    // turns off all LEDs
         }
         */
        // end revese direction
    }

}












