#include "Mesh_Node.h"

static RF24 radio(7, 8);
const byte READ_ADDRESS[6] = { 1 };
const byte WRITE_ADDRESS[6] = { 0 };

int mapIndex = 0;


Mesh_Node::Mesh_Node(int channel) :_channel(channel), _packageCount(0),_initFlag(false),_uniqueId(2), _packageListSize(0)
{
	_camera = new Camera();
	_packageList = LinkedList<PackageWraper*>();
}

bool Mesh_Node::init()
{
	bool status = false;
	_camera->init();
	radio.begin();
	Wire.begin();
	_gps.init();
	radio.setChannel(_channel);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);
	radio.setAutoAck(false);
	radio.openReadingPipe(1, READ_ADDRESS[0]);
	radio.openWritingPipe(WRITE_ADDRESS);
	radio.startListening();
	_initFlag = true;

	return status;
}

bool Mesh_Node::sendMessage(Package package)
{
	Serial.println("SendPackage");
	int status = false;
	if (radio.available()) {
		//stop listening before sending the package 
		radio.stopListening();

		if (radio.write(&package, sizeof(package))) {
			status = true;
		}
		//start listening for new packages
		radio.startListening();
	}

	return status;
}

bool Mesh_Node::readMessage()
{
	bool status = false;
	Package package;
	if (radio.available()) {
		radio.read(&package, sizeof(package));

		status = processMessage(package);
	}
	return status;
}

bool Mesh_Node::processMessage(Package& package)
{
	if (!validatePackageNotDuplicate(package)) {
		return;
	}

	addPackageToList(package);

	//if the package destination is the node
	if (package.addresses.b == _uniqueId)
	{
		switch (package.opcodeAndPriority.a)
		{
			//MOVE Request
			case MOVE_REQUEST:
			{
				//Sends raw data to controller for further processing
				char command[5];
				for (int i = 0; i < 5; ++i) {
					command[i] = package.data[i];
				}
				Wire.beginTransmission(8);
				Wire.write(command, 5);
				Wire.endTransmission();

				break;
			}
			//GPS Request
			case GPS_REQUEST:
			{
				String str = "";
				if (_gps.GetGPSData(str)) {
					segmentPackageAndSend(str.c_str(), str.length(), GPS_RESPONSE);
				}

				break;
			}
			//Picture Request
			case PICTURE_REQUEST:
			{
				String str = "";
				if (_gps.GetGPSData(str)) {
					char* picture;
					_camera->takePicture(str.c_str());
					int size = _camera->getPicture(picture);
					segmentPackageAndSend(picture, size, PICTURE_RESPONSE);
					free(picture);
				}
				break;
			}
		}
	}
	else {
		//if the message is a response forward it
		forwardMessage(package);
	}

	return false;
}

void Mesh_Node::segmentPackageAndSend(const char* data, int size, char opcode) {
	char buffer[PACKAGE_MAX_DATA_SIZE];
	int segments = size / PACKAGE_MAX_DATA_SIZE;

	if ((size % PACKAGE_MAX_DATA_SIZE) > 0) {
		++segments;
	}

	int segmentNum = 1;
	int index = 0;

	while (size != index) {
		int remaining = size - index;
		int bufferSize;
		if (remaining > PACKAGE_MAX_DATA_SIZE)
			bufferSize = PACKAGE_MAX_DATA_SIZE;
		else
			bufferSize = remaining;
		memcpy(buffer, data + index, bufferSize);

		Package p = createPackage(HQ_ADDRESS, opcode, buffer, bufferSize, segments > 0 ? segments : 0, segmentNum++);
		sendMessage(p);
		addPackageToList(p);
		index += bufferSize;
		delay(500);
	}
}

bool Mesh_Node::forwardMessage(Package& package)
{
	Serial.println("Forward");
	bool status = false;
	//check if hop count is smaller than max hop count
	if (package.hops.a > package.hops.b) {
		package.hops.b++;
		status = sendMessage(package);
	}
	else {
		Serial.println("ignoring message");
		status = true;
	}
	return status;
}

bool Mesh_Node::isInitComplete()
{
	return _initFlag;
}


bool Mesh_Node::validatePackageNotDuplicate(Package& package)
{


	for (int i = 0; i < _packageList.size(); ++i) {
		if (*(_packageList.get(i)) == package) {
			return false;
		}
	}
	return true;
}

void Mesh_Node::addPackageToList(Package& package)
{
	PackageWraper* packageWraper = new PackageWraper(package);
	if (_packageList.add(packageWraper)) {
		++_packageListSize;
	}

	return;
}

Package Mesh_Node::createPackage(int destinationAddress, char opcode, char * data, int length, int numOfSegments = 0, int segmentNumber = 0)
{
	return createPackage(destinationAddress, opcode, data, length, MEDIUM_PRIORITY, MEDIUM_PROPAGATION, numOfSegments, segmentNumber);
}


Package Mesh_Node::createPackage(int destinationAddress, char opcode, char * data, int length, byte prioprity, byte propegation,  int numOfSegments = 0, int segmentNumber = 0)
{
	Package package;
	Serial.println(data);
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
	package.opcodeAndPriority.b = prioprity;
	package.opcodeAndPriority.c = propegation;
	memcpy((char*)package.data, (char*)data, length);
	return package;
}



void Mesh_Node::printPackage(Package& package)
{
		Serial.print("packageId = ");
		Serial.println(package.packageId);

		Serial.print("addresses originator = ");
		Serial.println(package.addresses.a);

		Serial.print("addresses destination = ");
		Serial.println(package.addresses.b);

		Serial.print("hops max = ");
		Serial.println(package.hops.a);
		
		Serial.print("hops current = ");
		Serial.println(package.hops.b);

		Serial.print("opcodeAndPriority opcode = ");
		Serial.println(package.opcodeAndPriority.a);

		Serial.print("opcodeAndPriority priority = ");
		Serial.println(package.opcodeAndPriority.b);

		Serial.print("opcodeAndPriority propagation= " );
		Serial.println(package.opcodeAndPriority.c);

		Serial.print("segments total = ");
		Serial.println(package.segments.a);

		Serial.print("segments num = " );
		Serial.println(package.segments.b);

		Serial.print("data = ");
		Serial.println(package.data);

}

void Mesh_Node::printPackage(PackageWraper& package)
{

	printPackage(package.getPackage());

	Serial.print("priority = ");
	Serial.println(package.getPriority());

	Serial.print("propegation = ");
	Serial.println(package.getPropegation());

}

bool Mesh_Node::isDataAvailable()
{
	return radio.available();

}

void Mesh_Node::propegatePacakges()
{
	Serial.println("propegatePacakges");

	for (int i = 0; i < _packageListSize; ++i) {
		PackageWraper* tmp = _packageList.remove(i);
		//if this package was sent less then the propegation number it is active
		if (tmp->isActive()) {
			//reduce priority by one
			int priority = tmp->getPriority();
			--priority;
			tmp->setPriority(priority);
			//if time has come to send again the message send it and reduce propegation by 1
			if (priority == 0) {

				sendMessage(tmp->getPackage());
				tmp->setPriority(tmp->getPackage());

				//if propegation is larger then 1 reduce 1 since we sent the message again
				int propegation = tmp->getPropegation();
				if (propegation > 1) {
					--propegation;

				}
				//if propegation is equal or smaller then 1  set active = false;
				else
					tmp->setActive(false);
			}
		}
		_packageList.add(tmp);
	}
}

Mesh_Node::~Mesh_Node()
{
	for (int i = _packageListSize; i > 0 ; --i) {
		free(_packageList.remove(i));
	}
	free(_camera);
}


