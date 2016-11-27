/**
 * simple IRnode to control NEC compatible devices via V_VAR1
 * 
 * original code by Ken Shirriff - thanks a lot!
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
#include <IRremote.h>  // https://github.com/z3t0/Arduino-IRremote/releases   
// OR install IRRemote via "Sketch" -> "Include Library" -> "Manage Labraries..."
// Search for IRRemote b shirif and press the install button

#define SKETCH_NAME "IRnode"           // Name
#define SKETCH_VER "1.2"               // Version

#define CHILD_ID_IR 0                  // Child ID IR

IRsend irsend;

MyMessage msgIr(CHILD_ID_IR, V_VAR1);

// ********** SETUP **********************************************************************

void setup()
{ 

}

// ********** PRENSENTATION **************************************************************

void presentation () 
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VER);

  // Register a sensors to gw. Use binary light for test purposes.
  present(CHILD_ID_IR, V_VAR1);
}

// ********** LOOP ***********************************************************************

void loop()
{

}

// ********** RECEIVE ********************************************************************

void receive(const MyMessage &message) {
  if (message.type==V_VAR1) {
     long Code = message.getULong();
     irsend.sendNEC(Code, 32);
  }
}

// ***************************************************************************************
