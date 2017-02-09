/**
 * moodlight v2.1
 * 
 * original code by sweebee ( https://github.com/sweebee/Arduino-home-automation/tree/master/MySensors%201.5/Arduino/Moodlight ) - thanks a lot!
 * 
 * modified by m1
 * 
 * works with mysensors protocol 2.0.0
 */

// ********** CONFIG *********************************************************************

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_NODE_ID 20                  // Node ID

#include <SPI.h>
#include <MySensors.h>
#include <Adafruit_NeoPixel.h>

#define SKETCH_NAME "Moodlight"        // Name
#define SKETCH_VER "2.1"               // Version

//#define CHILD_ID_LIGHT 0               // Child ID
#define LED_PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, LED_PIN, NEO_GRB + NEO_KHZ800);

int R;
int G;
int B;

int Rold;
int Gold;
int Bold;

int currentLevel = 0; // Dimlevel
long RGB_values[3] = {0, 0, 0}; // Colors

MyMessage dimmerMsg(0, V_DIMMER);

// ********** SETUP **********************************************************************

void setup()
{ 

  strip.begin();
  strip.show();

  colorChange(255, 0, 255, true); // Purple
  colorChange(0, 0, 0, true); // Off

}

// ********** PRENSENTATION **************************************************************

void presentation () 
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VER);

  // Register a sensors to gw. Use binary light for test purposes.
  present(0, S_CUSTOM);
  present( 0, S_DIMMER );
}

// ********** LOOP ***********************************************************************

void loop()
{

}

// ********** RECEIVE ********************************************************************

void receive(const MyMessage &message) {

  if (message.type == V_VAR1) {
    String hexstring = message.getString();

    // Check if contains hex character

      // Remove the character
      hexstring.remove(0, 1);
    
    Serial.println(hexstring);
    long number = (long) strtol( &hexstring[0], NULL, 16);
    RGB_values[0] = number >> 16;
    RGB_values[1] = number >> 8 & 0xFF;
    RGB_values[2] = number & 0xFF;
    
    R = RGB_values[0];
    G = RGB_values[1];
    B = RGB_values[2];
    colorChange(R, G, B, true);

    // Write some debug info
    Serial.print("Red is " );
    Serial.println(RGB_values[0]);
    Serial.print("Green is " );
    Serial.println(RGB_values[1]);
    Serial.print("Blue is " );
    Serial.println(RGB_values[2]);
  }

  if (message.type == V_DIMMER) {
    // Get the dimlevel
    int reqLevel = atoi( message.data );
    // Clip incoming level to valid range of 0 to 100
    reqLevel = reqLevel > 100 ? 100 : reqLevel;
    reqLevel = reqLevel < 0   ? 0   : reqLevel;

    // Set brightness
    Serial.print("Dimming to ");
    Serial.println(reqLevel);
    fadeToLevel(reqLevel);
    
}

}

// ***************************************************************************************

// CHANGE TO COLOR
void colorChange(uint32_t R, uint32_t G, uint32_t B, uint32_t fade) {
  if (!fade) {
    Rold = R;
    Bold = B;
    Gold = G;
    for (int i = 0; i < 16; i++) {
      strip.setPixelColor(i, strip.Color(Rold, Gold, Bold));
    }
    strip.show();
  } else {
    for (int t = 0; t < 256; t++) {
      if (R > Rold) {
        Rold++;
      }
      if (R < Rold) {
        Rold--;
      }
      if (G > Gold) {
        Gold++;
      }
      if (G < Gold) {
        Gold--;
      }
      if (B > Bold) {
        Bold++;
      }
      if (B < Bold) {
        Bold--;
      }
      for (int i = 0; i < 16; i++) {
        strip.setPixelColor(i, strip.Color(Rold, Gold, Bold));
      }
      strip.show();
      delay(2);
    }
  }
}

// DIMMER FADE
void fadeToLevel( int toLevel ) {

  int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;

  while ( currentLevel != toLevel ) {
    currentLevel += delta;
    strip.setBrightness(currentLevel);
    if(currentLevel == 1){
      for (int i = 0; i < 16; i++) {
        strip.setPixelColor(i, strip.Color(Rold, Gold, Bold));
      }
    }
    strip.show();
    delay(15);
  }
}
