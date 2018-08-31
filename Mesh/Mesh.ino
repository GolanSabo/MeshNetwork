#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LinkedList.h>
#include <SPI.h>
#include <Wire.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include "Mesh_Node.h"
static const int CHANNEL = 115;
Mesh_Node node(CHANNEL);

void setup()
{
	Serial.begin(9600);
	node.init();
}
bool flag = true;
void loop()
{

	if (node.isInitComplete()) {

		if (node.isDataAvailable())
			node.readMessage();
		node.propegatePacakges();
	}
	
	delay(5000);
}
