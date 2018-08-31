#pragma once

#include "Package.h"
class HQ_Node
{
	int _packageCount;
	int _uniqueId;
	int _channel;
public:
	HQ_Node(int channel);
	bool init();
	Package createPackage(int destinationAddress, char opcode, const char * data, int length, int numOfSegments, int segmentNumber);
	bool sendMessage(Package package);
	bool sendMessageToHQ(Package package);
	~HQ_Node();
};

