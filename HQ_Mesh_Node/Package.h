#pragma once
#pragma once
#define PACKAGE_MAX_DATA_SIZE 27
#include <RF24.h>
typedef struct {
	unsigned char a : 4;
	unsigned char b : 4;
}Address, Hops, Segment;

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

			Serial.println("equals");
			return true;
		}
		//else {
		//	Serial.println("not equals");
		//	Serial.print("packageId = ");
		//	Serial.println(package.packageId);
		//	Serial.println(packageId);
		//	Serial.println(package.packageId == packageId);

		//	Serial.print("addresses originator = ");
		//	Serial.println(package.addresses.a);
		//	Serial.println(addresses.a);
		//	Serial.println(package.addresses.a == addresses.a);

		//	Serial.print("addresses destination = ");
		//	Serial.println(package.addresses.b);
		//	Serial.println(addresses.b);
		//	Serial.println(package.addresses.b == addresses.b);


		//	Serial.print("hops max = ");
		//	Serial.println(package.hops.a);
		//	Serial.println(hops.a);
		//	Serial.println(package.hops.a == hops.a);

		//	Serial.print("hops current = ");
		//	Serial.println(package.hops.b);
		//	Serial.println(hops.b);
		//	Serial.println(package.hops.b == hops.b);

		//	Serial.print("opcodeAndPriority opcode = ");
		//	Serial.println(package.opcodeAndPriority.a);
		//	Serial.println(opcodeAndPriority.a);
		//	Serial.println(package.opcodeAndPriority.a == opcodeAndPriority.a);

		//	Serial.print("opcodeAndPriority priority = ");
		//	Serial.println(package.opcodeAndPriority.b);
		//	Serial.println(opcodeAndPriority.b);
		//	Serial.println(package.opcodeAndPriority.b == opcodeAndPriority.b);

		//	Serial.print("opcodeAndPriority propagation= ");
		//	Serial.println(package.opcodeAndPriority.c);
		//	Serial.println(opcodeAndPriority.c);
		//	Serial.println(package.opcodeAndPriority.c == opcodeAndPriority.c);

		//	Serial.print("segments total = ");
		//	Serial.println(package.segments.a);
		//	Serial.println(segments.a);
		//	Serial.println(package.segments.a == segments.a);


		//	Serial.print("segments num = ");
		//	Serial.println(package.segments.b);
		//	Serial.println(segments.b);
		//	Serial.println(package.segments.b == segments.b);

		//	Serial.print("data = ");
		//	Serial.println(package.data);
		//	Serial.println(data);
		//}

		//Serial.print("Returning  = ");
		//Serial.println(status);

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
	
	//packageId,originatorAddress, DestinationAddress, Opcode, MaxHops, HopCount, Priority, Propegation, TotalSegments, SegmentNumber, Data;
	inline String toString() {
		String str = "";
		str += (packageId & 0x0F);
		str += ",";
		str += (addresses.a & 0x0F);
		str += ",";
		str += (addresses.b & 0x0F);
		str += ",";
		str += (opcodeAndPriority.a & 0x0F);
		str += ",";
		str += (hops.a & 0x0F);
		str += ",";
		str += (hops.b & 0x0F);
		str += ",";
		str += (opcodeAndPriority.a & 0x0F);
		str += ",";
		str += (opcodeAndPriority.c & 0x0F);
		str += ",";
		str += (segments.a & 0x0F);
		str += ",";
		str += (segments.b & 0x0F);
		str += ",";
		str += data;
		str += '\n';
		

		return str;
	}

	inline void printPackage() {
		Serial.println("packageId = " + packageId);
		Serial.println("addresses.a = " + addresses.a);
		Serial.println("addresses.b = " + addresses.b);
		Serial.println("hops.a = " + hops.a);
		Serial.println("hops.b = " + hops.b);
		Serial.println("opcodeAndPriority.a = " + opcodeAndPriority.a);
		Serial.println("opcodeAndPriority.b = " + opcodeAndPriority.b);
		Serial.println("opcodeAndPriority.c = " + opcodeAndPriority.c);
		Serial.println("segments.a = " + segments.a);
		Serial.println("segments.b = " + segments.b);
	}
};