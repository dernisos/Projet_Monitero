//Exemple created for the Adafruit Feather 32u4 RFM69HCW board
//Node of a Star Network
//This Part of the Exemple is used with the exemple Gateway.ino
#include <Adafruit_SleepyDog.h>
#include <RFM69.h>
#include <SPI.h>


#define NETWORKID		  100
#define NODEID			  3
#define GATEWAYID 	  254
#define FREQUENCY		  RF69_868MHZ
//Encrypted Key to send and received Data, need to be the same on gateway and node
#define ENCRYPTKEY		"sampleEncryptKey"
//Needed to put the High Power on the board
#define IS_RFM69HCW   true

#define SERIAL_BAUD 	115200


//for Feather 32u4 Radio
#define RFM69_CS		  8 //IDE PIN for the Chip Select
#define RFM69_IRQ		  7 //IDE PIN for the Interupt PIN
#define RFM69_RST		  4 //IDE PIN for the Reset PIN
#define VBATPIN       A9 //IDE PIN for the Battery Input
#define LED				    13 //IDE PIN for the LED


RFM69 radio(RFM69_CS,RFM69_IRQ, IS_RFM69HCW);
unsigned long previousTime;
unsigned long tempoTime;

typedef struct
{
  unsigned long TransmitPeriod;
} changeMessage;
changeMessage thisChMess;

typedef struct 
{
	int				    nodeID;		//This nodeID
	unsigned long	upTime;		//upTime in ms
	float 			  dataSensor; //value given by the sensor
  char          typeCapteur; //Type de capteur
} Payload;
Payload thisData;


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
  thisChMess.TransmitPeriod = 5000;
  thisData.typeCapteur = '1';
  //Initialize the Time compair variable
  tempoTime=millis();
}

void loop()
{
	
	//check for any received packets
	//TODO check if it's the gateway that send the message
	if(radio.receiveDone())
  {
    Payload tempData = *(Payload*)radio.DATA;
    if(tempData.nodeID != GATEWAYID)
    {
      //print RSSI(Relative Receive Signal Strength) to serial
      Serial.print("  [RX_RSSI:");  Serial.print(radio.readRSSI()); Serial.println("]"); 
        
      //Check if an ACK is requested and send the answer to gateway
      //TODO personalize this part to switch speed of transmission
      if(radio.ACKRequested())
      {
        radio.sendACK();
        Serial.print(" - ACK sent");
        delay(10);
      }
    }
    else
    {
      
      //TODO vérifier les changement possible  
    }
    Blink(LED,5);
    Serial.println();
  }
  unsigned long elapsedTime = previousTime-millis();
  Serial.print("ElapsedTime : ");Serial.println(elapsedTime);
	Serial.print("TransmitPeriod : ");Serial.println(thisChMess.TransmitPeriod);
	//unsigned long elapsedTime = (millis()-tempoTime)+previousTime;
	//Check if the transmit period is up to transmit 
	if(elapsedTime > thisChMess.TransmitPeriod)
	{
		//fill in the Data structure
		thisData.nodeID = NODEID;
		thisData.upTime = elapsedTime;
    //TODO récup sensor
		thisData.dataSensor = GetTemp(23);
		
		Serial.print("Sending Data of ");
		Serial.print(sizeof(thisData));
		Serial.println(" Bytes ...");
		//Sending of the data
		if(radio.sendWithRetry(GATEWAYID, (const void*)(&thisData),sizeof(thisData)))
			Serial.print("Done!");
		else
			Serial.print("Failure...");
		Serial.println();
		Blink(LED,3); 
    
    delay(1000);
    //Take the response of the Gateway with possible change
    if(radio.receiveDone()){
      Serial.print("Sender node : ");Serial.print(radio.SENDERID, DEC);
      Serial.print(" with RX_RSSI of ");Serial.println(radio.readRSSI());
      if(radio.DATALEN == sizeof(thisChMess) && radio.SENDERID == GATEWAYID){
        changeMessage tempChMess = *(changeMessage*)radio.DATA;
        thisChMess.TransmitPeriod = tempChMess.TransmitPeriod;
      }
    }
		//Reinitialization of the previousTime value
		//previousTime = 0;
    previousTime = millis();
	}
  //tempoTime = millis();
  //radio.sleep(); //Put RFM69HCW module on sleep mode, It wake up when we call receiveDone()  
  //previousTime = previousTime + millis()-tempoTime;
  //previousTime = previousTime + Watchdog.sleep(1000); //Put in sleep mode Mega32u4 for 1000ms
  //tempoTime=millis();

}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

float GetTemp(byte PIN){
  double tempReading = analogRead(PIN);
  double tempK = log(10000.0 /*Resistance de pompage*/ * (1024.0/*Max on CAN*/ / tempReading-1));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  return tempC;
}

float GetBattery(void){
  float measurebat = analogRead(VBATPIN);
  measurebat *= 2;
  measurebat *= 3.3;
  measurebat /= 1024;
  return ((measurebat - 3.0)*100)/0.7;
}

