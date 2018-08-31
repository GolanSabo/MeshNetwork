#pragma once
#define PACKAGE_MAX_DATA_SIZE 27
#include <RF24.h>
typedef struct{
	unsigned char a : 4;
	unsigned char b : 4;
}Address,Hops,Segment;

typedef struct {
	unsigned char a : 4;
	unsigned char b : 2;
	unsigned char c : 2;
}OpcodeAndPriority;

struct Package {
	unsigned char packageId;
	//MSB 4 bits are originator address and LSB 4 bits are destination address
	Address addresses;
	//MSB 4 bits are max hop count and LSB 4 bits are hops counted
	Hops hops;
	//MSB 4 bits are the opcode of the package next 2 bits are package priority and LSB 2 bits are propegation interval
	OpcodeAndPriority opcodeAndPriority;
	//MSB 4 bits are the total number of segments for this package LSB 4 bits are the segment number of this package
	Segment segments;
	char data[PACKAGE_MAX_DATA_SIZE];

	inline Package operator=(Package a) {
		packageId = a.packageId;
		addresses.a = a.addresses.a;
		addresses.b = a.addresses.b;
		hops.a = a.hops.a;
		hops.b = a.hops.b;
		opcodeAndPriority.a = a.opcodeAndPriority.a;
		opcodeAndPriority.b = a.opcodeAndPriority.b;
		opcodeAndPriority.c = a.opcodeAndPriority.c;
		segments.a = a.segments.a;
		segments.b = a.segments.b;
		memcpy(data, a.data, sizeof(a.data));

		return a;
	}

	inline bool operator==(Package package) {
		bool status = false;
		if (segments.b == package.segments.b && packageId == package.packageId && addresses.b == package.addresses.b && hops.a == package.hops.a && hops.b == package.hops.b && opcodeAndPriority.a == package.opcodeAndPriority.a && opcodeAndPriority.b == package.opcodeAndPriority.b && opcodeAndPriority.c == package.opcodeAndPriority.c && segments.a == package.segments.a) {
			
			return true;
		}
			return status;
		
	}


	inline int operator=(int a) {
		packageId = a;
		addresses.a = a;
		addresses.b = a;
		hops.a = a;
		hops.b = a;
		opcodeAndPriority.a = a;
		opcodeAndPriority.b = a;
		opcodeAndPriority.c = a;
		segments.a = a;
		segments.b = a;
		return a;
	}

};