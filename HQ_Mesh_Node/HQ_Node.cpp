#include "HQ_Node.h"

static RF24 radio(7, 8);
const byte READ_ADDRESS[6] = { 0 };
const byte WRITE_ADDRESS[6] = { 1 };

HQ_Node::HQ_Node(int channel):_uniqueId(1),_packageCount(1),_channel(channel)
{
	Serial.begin(115200);
}

bool HQ_Node::init()
{
	bool status = false;
	radio.begin();
	radio.setChannel(_channel);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);
	radio.setAutoAck(false);
	radio.openReadingPipe(1, READ_ADDRESS[0]);
	radio.openWritingPipe(WRITE_ADDRESS);//open writing pipe to address pipe 1
	radio.startListening();
	//Serial.println("Init complete");
	return status;
}

Package HQ_Node::createPackage(int destinationAddress, char opcode, const char * data, int length, int numOfSegments = 0, int segmentNumber = 0)
{
	Package package;
	if (numOfSegments > segmentNumber) {
		package.packageId = _packageCount;
		package.segments.a = numOfSegments;
		package.segments.b = segmentNumber;
	}
	else {
		package.packageId = (_packageCount++) % 255;
		package.segments.a = 0;
		package.segments.b = 0;
	}

	package.addresses.a = _uniqueId;
	package.addresses.b = destinationAddress;
	package.hops.a = 5;
	package.hops.b = 0;
	package.opcodeAndPriority.a = opcode;
	package.opcodeAndPriority.b = 1;
	package.opcodeAndPriority.c = 1;

	memcpy((char*)package.data, (char*)data, length);
	return package;
}

bool HQ_Node::sendMessage(Package package)
{
	int status = false;
	//stop listening before sending the package 
	radio.stopListening();
	if (radio.write(&package, sizeof(package))) {
		status = true;
	}
	//start listening for new packages
	radio.startListening();
	return status;
}


bool HQ_Node::sendMessageToHQ(Package package)
{
	bool status = false;
	Serial.flush();
	Serial.write(package.toString().c_str());
	return status;
}

HQ_Node::~HQ_Node()
{
}


