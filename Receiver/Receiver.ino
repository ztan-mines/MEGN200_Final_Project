#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);

const byte address[6] = "00001";
int average;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if(radio.available()){
    average = 0;
    radio.read(&average, sizeof(average));

    // print received data
    Serial.print(average);  // TODO: append data w/ timestamp to file
    Serial.println("cm");
  }
}
