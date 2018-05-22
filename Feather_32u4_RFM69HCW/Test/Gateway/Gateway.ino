//Exemple created for the Adafruit Feather 32u4 RFM69HCW board
//Gateway of a Star network
//This Part of the Exemple is used with the exemple Node.ino
#include <RFM69.h>
#include <SPI.h>


#define NETWORKID		  100
#define NODEID			  254
#define FREQUENCY		  RF69_868MHZ
//Encrypted Key to send and received Data, need to be the same on gateway and node
#define ENCRYPTKEY		"sampleEncryptKey"
//Needed to put the High Power on the board
#define IS_RFM69HCW   true
#define SERIAL_BAUD 	115200


//for Feather 32u4 Radio
#define RFM69_CS      8 //IDE PIN for the Chip Select
#define RFM69_IRQ     7 //IDE PIN for the Interupt PIN
#define RFM69_RST     4 //IDE PIN for the Reset PIN

#define LED           13 //IDE PIN for the LED


RFM69 radio = RFM69( RFM69_CS, RFM69_IRQ, IS_RFM69HCW );

unsigned long ChTransmitPeriod;

typedef struct
{
	unsigned long TransmitPeriod;
} changeMessage;
changeMessage thisChMess;

typedef struct 
{
	int				    nodeID;		//This nodeID
	unsigned long		upTime;		//upTime in ms
	float 			  	dataSensor; //value given by the sensor
	char          		typeCapteur; //Type de capteur
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
	thisChMess.TransmitPeriod = 119000;
	ChTransmitPeriod = thisChMess.TransmitPeriod;
	Serial.println( "Listening at 868 MHz...");
}

void loop(){
  // Devra être remplacé par la réception d'un changement par GSM
  if(Serial.available() > 0)
  {
    char input = Serial.read();
    if(input == 'a')
    {  
      ChTransmitPeriod = 1000;
    }
    if(input == 'z')
    {  
      ChTransmitPeriod = 5000;
    }
    if(input == 'e')
    {  
      ChTransmitPeriod = 10000;
    }
    if(input == 'r')
    {  
      ChTransmitPeriod = 20000;
    }
  }
	//check for any received packets
	if(radio.receiveDone())
	{
		Serial.print("Sender node : ");Serial.print(radio.SENDERID, DEC);
		Serial.print(" with RX_RSSI of ");Serial.println(radio.RSSI);
		if(radio.DATALEN != sizeof(Payload))
			Serial.print("Invalid format of Data message");
		else
		{
			thisData = *(Payload*)radio.DATA; //We assume that the data send contain our format of data
			Serial.print("Capteur:");Serial.print(thisData.typeCapteur);Serial.print(",numero:");Serial.print(thisData.nodeID);
			Serial.print(",upTime:");Serial.print(thisData.upTime);
			Serial.print(",Sensor value:");Serial.print(thisData.dataSensor);
			//TODO send the data on Internet with GSM card
		}
		
		if(radio.ACKRequested())
		{
			byte thisNodeId = radio.SENDERID;
			radio.sendACK();
			
		}
		Serial.println();
		if(ChTransmitPeriod != thisChMess.TransmitPeriod){
      thisChMess.TransmitPeriod = ChTransmitPeriod;
      byte theNodeID = radio.SENDERID;
      if(radio.sendWithRetry(3, (const void*)(&thisChMess),sizeof(thisChMess)))
        Serial.print("Done!");
      else
        Serial.print("Failure...");  
      Serial.println();
    }
	}
 
  
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
