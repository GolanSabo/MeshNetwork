#pragma once
#include "HQ_Node.h"
static const int CHANNEL = 115;
HQ_Node node(CHANNEL);

void setup()
{
	Serial.begin(115200);
	node.init();
}
void loop()
{
	//Package package = node.createPackage(1, 3, "yay", 4, 3, 0);
	////package.printPackage();
	////Serial.println(package.toString());
	//node.sendMessageToHQ(package);
	delay(1000);
}

void serialEvent()
{
	if (Serial.available())
	{
		String data = Serial.readStringUntil('\n');
		//Serial.println("data = " + data);
		//originatorAddress, DestinationAddress, Opcode, MaxHops, HopCount, Priority, Propegation, TotalSegments, SegmentNumber, Data;
		String parsedData[10];
		int index1 = 0, index2 = 0, counter = 0;
		for (int i = 0; i < data.length(); ++i) {
			if (data.charAt(i) == ',') {
				index1 = i;
				for (int j = 0; j < index1 - index2; ++j) {
					parsedData[counter] += data[index2 + j];
				}
				counter++;
				index2 = index1 + 1;
			}
		}
		for (int j = 0; j < data.length() - index2; ++j) {
			parsedData[counter] += data[index2 + j];
		}
		for(int i = 0; i < 10; ++i){
			Serial.flush();
			Serial.write(parsedData[i].c_str());
		}
		
		Package package = node.createPackage((int)parsedData[1].toInt(), parsedData[2].charAt(0), parsedData[9].c_str(), parsedData[9].length(), parsedData[7].toInt() , parsedData[8].toInt());
		node.sendMessage(package);
	}


}