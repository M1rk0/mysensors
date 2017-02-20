#define MY_DEBUG
#define MY_RADIO_NRF24

#define MY_NODE_ID 22

#include <SPI.h>
#include <MySensors.h>

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define SLEEP_TIME 60000 // Sleep time between reads (in milliseconds)
#define STABILIZATION_TIME 500 // Let the sensor stabilize 0.5 seconds before reading

const int SENSORS[] = {A0, A1}; // Remove the pins that you don't want to use
#define N_ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 2 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay

void before() { 
  for (int sensor = 1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    pinMode(pin, OUTPUT);   
    digitalWrite(pin, RELAY_OFF);
  }
}

MyMessage moisture_messages[N_ELEMENTS(SENSORS)];

void setup()
{
  for (int i = 0; i < N_ELEMENTS(SENSORS); i++) {
    pinMode(SENSORS[i], OUTPUT);
    digitalWrite(SENSORS[i], LOW);
  }
}

void presentation()
{
  
  sendSketchInfo("2SoilMoisture-2Relay", "1.0");
  
  for (int sensor = 1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    present(sensor, S_SPRINKLER);
  }
  
  for (int sensor = 0; sensor < N_ELEMENTS(SENSORS); sensor++) {
    moisture_messages[sensor].sensor = sensor + 3;
    moisture_messages[sensor].type = V_HUM;
    delay(250);
    present(sensor + 3, S_HUM);
  }
  
}

void loop() {
  
}

void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_VAR1) {
     String wert = message.getString();
     if (wert == "1") {
      digitalWrite(message.sensor-1+RELAY_1, RELAY_ON);
      wait(1000);
      digitalWrite(message.sensor-1+RELAY_1, RELAY_OFF);
     }
   } 
  if (message.type==V_VAR2) {
    for (int sensor = 0; sensor < N_ELEMENTS(SENSORS); sensor++) {
      pinMode(SENSORS[sensor], INPUT_PULLUP); // "Power on" the sensor and activate the internal pullup resistor
      analogRead(SENSORS[sensor]); // Read once to let the ADC capacitor start charging
      sleep(STABILIZATION_TIME);
      int moistureLevel = (1023 - analogRead(SENSORS[sensor])) / 10.23;
      
      pinMode(SENSORS[sensor], OUTPUT);
      digitalWrite(SENSORS[sensor], LOW);
      
      send(moisture_messages[sensor].set(moistureLevel));
    }
   }  
}
