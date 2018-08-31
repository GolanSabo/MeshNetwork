#include "Camera.h"

const int CS_PIN = 4;
Camera::Camera() :_lastPictureName("")
{

}

void Camera::init() 
{
	pinMode(CS_PIN, OUTPUT);
	digitalWrite(CS_PIN, HIGH);

	if (SD.begin(CS_PIN))
	{
		Serial.println("SD card is ready to use.");
	}
	else
	{
		Serial.println("SD card initialization failed");
		return;
	}
}

void Camera::takePicture(const char* fileName)
{
	_lastPictureName = fileName;
	_lastPictureName += ".txt";

	_picture = SD.open(_lastPictureName, FILE_WRITE);
	//Write input from camera

	_picture.close();
	
}

int Camera::getPicture(char* buffer)
{
	_picture = SD.open(_lastPictureName, FILE_READ);

	char* data = new char[1];
	String str = "";
	while (_picture.readBytes(data, 1) > 0)
	{
		str += data;
	}
	buffer = new char[str.length()];
	memcpy(buffer, str.c_str(), str.length());

	_picture.close();
	return str.length();
}


Camera::~Camera()
{
}
