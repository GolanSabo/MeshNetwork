#pragma once
//#include "Package.h"
#include <RF24.h>
#include <Wire.h>
#include <stdio.h>
#include "GPS.h"
#include "Camera.h"
#include "HashMap.h"
#include <LinkedList.h>
#include "PackageWraper.h"
#define HQ_ADDRESS 1
#define MAX_NUM_OF_MESSAGES 200
enum Opcode { MOVE_REQUEST = 0, GPS_REQUEST, PICTURE_REQUEST, GPS_RESPONSE, PICTURE_RESPONSE, VIDEO_PACKAGE };
class Mesh_Node
{
	byte _uniqueId;
	const int _channel;
	int _packageCount;
	Camera* _camera;
	GPS _gps;
	bool _initFlag;
	LinkedList<PackageWraper*> _packageList;
	//returns true if duplicate, false if not duplication
	bool validatePackageNotDuplicate(Package& package);
	void addPackageToList(Package& package);
	bool sendMessage(Package package);
	int _packageListSize = 0;

public:


	Mesh_Node(int channel);
	bool init();
	bool readMessage();
	bool processMessage(Package& package);
	void segmentPackageAndSend(const char* picture, int size, char opcode);
	bool forwardMessage(Package& package);
	bool isInitComplete();
	Package createPackage(int destinationAddress,char opcode,char* data, int length, int numOfSegments = 0, int segmentNumber = 0);
	Package createPackage(int destinationAddress, char opcode, char * data, int length, byte prioprity, byte propegation, int numOfSegments, int segmentNumber);
	void printPackage(Package& package);
	void printPackage(PackageWraper& package);
	bool isDataAvailable();
	void propegatePacakges();
	~Mesh_Node();
};

