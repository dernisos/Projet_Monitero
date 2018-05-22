//Exemple created for the Adafruit Feather 32u4 RFM69HCW board
//Test to of a Thermoresistor
#include <RFM69.h>
#include <SPI.h>


#define NETWORKID      100
#define NODEID        2
#define GATAWAYID     254
#define FREQUENCY     RF69_868MHZ
//Encrypted Key to send and received Data
#define ENCRYPTKEY    "sampleEncryptKey"
//Needed to put the High Power on the board
#define IS_RFM69HCW   true

#define SERIAL_BAUD   115200


//for Feather 32u4 Radio
#define RFM69_CS      8 //IDE PIN for the Chip Select
#define RFM69_IRQ     7 //IDE PIN for the Interupt PIN
#define RFM69_RST     4 //IDE PIN for the Reset PIN

#define LED           13 //IDE PIN for the LED


RFM69 radio(RFM69_CS,RFM69_IRQ, IS_RFM69HCW);
int TRANSMITPERIOD = 10000; //period to transmit a packet to gateway (in ms)
unsigned long previousTime;


typedef struct 
{
  int       nodeID;   //This nodeID
  unsigned long uptime;   //uptime in ms
  float       datasensor; //value given by the sensor
} Payload;
Payload thisData;

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
  delay(DELAY_MS);
}

float GetTemp(byte PIN){
  double tempReading = analogRead(PIN);
  double tempK = log(10000.0 /*Resistance de pompage*/ * (1024.0/*Max on CAN*/ / tempReading-1));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  return tempC;
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(10);
  
  //Hard Reset the RFM module
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);
  
  //Initialize radio
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  #ifdef IS_RFM69HCW
    radio.setHighPower();
  #endif
  radio.encrypt(ENCRYPTKEY);

  Serial.println("Sending at 868 MHz...");
  
  //Init the previousTime value
  previousTime = millis();
  Serial.flush();
}

void loop()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);
  Serial.println(GetTemp(23));
  delay(1000);
}

