#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);

#define ECHO_PIN 2
#define TRIG_PIN 3
#define VACANT_LED 10
#define OCCUPIED_LED 9

const byte address[6] = "00001";  // communication channel for transmitter & receiver
const int DISTANCES_SIZE = 10;
const int MEASUREMENT_DELAY = 1000;
const int CONVERT_TO_CM = 58;
const int TRIGGER_DISTANCE = 200; //TODO: research height of ceilings in parking garages
int distances[DISTANCES_SIZE];
int average = 0;

void setup() {
  Serial.begin(9600);

  // configure arduino for transmitting signals
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();  // transmitter

  // configure ultrasonic sensor
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(ECHO_PIN, HIGH);

  // configure LED pins
  pinMode(VACANT_LED, OUTPUT);
  pinMode(OCCUPIED_LED, OUTPUT);
}


int getDistance(){
  /**
   * Return a distance measurement from the ultrasonic sensor.
   */
  int distance = 0;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  distance = pulseIn(ECHO_PIN, HIGH, 26000);
  distance = distance / CONVERT_TO_CM;
  return distance;
}

bool isOccupied(){
  /**
   * Determine whether parking space is occupied
   */
  return (average > 0 && average < TRIGGER_DISTANCE);
}


void loop() {

  // get a measurement from the ultrasonic sensor
  for(int i = 0; i < DISTANCES_SIZE; ++i){
    distances[i] = getDistance();
  }

  // average values in distances array
  int total = 0;
  for(int i = 0; i < DISTANCES_SIZE; ++i){
    total += distances[i];
  }
  average = total / DISTANCES_SIZE;

  // send average distance reading to receiver
  radio.write(&average, sizeof(average));  

  // turn on appropriate indicator lights based on whether parking space is occupied
  if(isOccupied()){
    digitalWrite(VACANT_LED, LOW);
    digitalWrite(OCCUPIED_LED, HIGH);
  } else {
    digitalWrite(VACANT_LED, HIGH);
    digitalWrite(OCCUPIED_LED, LOW);
  }
  
  delay(MEASUREMENT_DELAY);  // wait some time before the next measurement  
}
