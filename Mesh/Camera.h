#pragma once
#include<string.h>
#include <RF24.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
class Camera
{
	File _picture;
	String _lastPictureName;
public:
	Camera();
	void init();
	void takePicture(const char* fileName);
	int getPicture(char* buffer);
	~Camera();
};

