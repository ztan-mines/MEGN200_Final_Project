#define ECHO_PIN 2
#define TRIG_PIN 3
#define VACANT_LED 13
#define OCCUPIED_LED 12

int distances[10];
const int MEASUREMENT_DELAY = 1000;
const int CONVERT_TO_CM = 58;
const int TRIGGER_DISTANCE = 200; //TODO: research height of ceilings in parking garages


void setup() {
  
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(ECHO_PIN, HIGH);

  pinMode(VACANT_LED, OUTPUT);
  pinMode(OCCUPIED_LED, OUTPUT);

}

void loop() {

  // TODO: adjust delay (we don't need measurements every few microseconds)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  distance = pulseIn(ECHO_PIN, HIGH, 26000);
  distance = distance / CONVERT_TO_CM;
  Serial.print(distance);
  Serial.println(" cm");

  if(distance > 0 && distance < TRIGGER_DISTANCE){
    digitalWrite(VACANT_LED, LOW);
    digitalWrite(OCCUPIED_LED, HIGH);
  } else {
    digitalWrite(VACANT_LED, HIGH);
    digitalWrite(OCCUPIED_LED, LOW);
  }
  
  delay(MEASUREMENT_DELAY);

}
