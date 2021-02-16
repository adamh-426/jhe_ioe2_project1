// Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include "pitches.h"
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/************************ Example Starts Here *******************************/

// digital pin 16
#define BUTTON_PIN 16
#define LED_PIN 13 

#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int val;

// button state
bool current = false;
bool last = false;

// set up the 'digital' feed
// change "myFeed" to the feed name you have created in Adafruit IO
AdafruitIO_Feed *projectOne = io.feed("projectOne"); //New!

int melody1[] = {
  NOTE_CS4, NOTE_GS5, NOTE_CS4, NOTE_F5, 0, 
  NOTE_CS5, NOTE_CS4, NOTE_DS5, NOTE_CS5, NOTE_CS4, NOTE_GS4, NOTE_AS4, NOTE_CS5, 
  NOTE_FS4, NOTE_AS4, NOTE_FS4, NOTE_CS5, 0, 
  NOTE_C5, NOTE_GS4, NOTE_CS5, NOTE_DS5, NOTE_GS4, 0,
  NOTE_CS4, NOTE_GS5, NOTE_CS4, NOTE_F5, 0, 
  NOTE_CS5, NOTE_CS4, NOTE_DS5, NOTE_CS5, NOTE_CS4, NOTE_GS4, NOTE_AS4, NOTE_CS5, 
  NOTE_FS5, NOTE_F5, NOTE_DS5, 0, NOTE_DS5, NOTE_CS5, NOTE_AS4, 0,
  NOTE_AS4, NOTE_CS5, NOTE_CS5, 0, NOTE_GS4,
  NOTE_CS4, NOTE_GS5, NOTE_CS4, NOTE_F5, 0, 
  NOTE_CS5, NOTE_CS4, NOTE_DS5, NOTE_CS5, NOTE_CS4, NOTE_GS4, NOTE_AS4, NOTE_CS5, 
  NOTE_FS4, NOTE_AS4, NOTE_FS4, NOTE_CS5, 0, 
  NOTE_C5, NOTE_GS4, NOTE_CS5, NOTE_DS5, NOTE_GS4, 0,
  NOTE_CS4, NOTE_GS5, NOTE_CS4, NOTE_F5, 0, 
  NOTE_CS5, NOTE_CS4, NOTE_DS5, NOTE_CS5, NOTE_CS4, NOTE_GS4, NOTE_AS4, NOTE_CS5, 
  NOTE_FS5, NOTE_F5, NOTE_DS5, 0, NOTE_DS5, NOTE_CS5, NOTE_AS4, 0,
  NOTE_AS4, NOTE_CS5, NOTE_CS5, 0, 
};

int noteDurations1[] = {
  4, 4, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 
  4, 4, 8, 4, 8, 
  8, 8, 8, 8, 4, 4, 
  4, 4, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 4, 8, 8,
  4, 4, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 
  4, 4, 8, 4, 8, 
  8, 8, 8, 8, 4, 4, 
  4, 4, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 4, 8, 
};

void setup() {

  // set button pin as an input, set led pin as output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); //New!

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // grab the current state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  if(digitalRead(BUTTON_PIN) == HIGH){
    current = true;
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);   
    rainbow(10);             // Flowing rainbow cycle along the whole strip
    Serial.println(val);
    val = map(val, 0, 1023, 95, 180);     // scale it to use it with the servo (value between 0 and 180)
  }else{
    current = false;
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
  // return if the value hasn't changed
  if(current == last){
    return;
  }
  
  // save the current state to the 'digital' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(current);

  //place your specific feed name
 projectOne->save(current); //New!

  // store last button state
  last = current;

}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
