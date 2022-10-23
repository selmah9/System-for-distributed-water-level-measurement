#include <LoRa.h>

/* Arduino Tutorial - Watel Level Sensor 40mm
   More info: */
#include <SPI.h>
#include <LoRa.h>

int counter = 0;

const int read = A0; //Sensor AO pin to Arduino pin A0
int value;          //Variable to store the incomming data
double treshold = 0.82;

double water_level = 0;

void setup()
{
  //Begin serial communication
  Serial.begin(9600);
   if (!LoRa.begin(868E6,4)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop(){
   LoRa.beginPacket();
  LoRa.print("-NV " + String(daj_nivo_vode()));
  LoRa.print(counter);
  LoRa.endPacket();
  Serial.println("poslao");
    counter++;
delay(2000);
}

double daj_nivo_vode(){
    return (double)analogRead(read)/1024;

    //  water_level = (double)analogRead(read)/1024;
//  Serial.println(water_level);
//  if(water_level > treshold) 
//     Serial.println("Nivo vode je iznad dozvoljenog!");
//  delay(1000); // Check for new value every 5 sec

 }
