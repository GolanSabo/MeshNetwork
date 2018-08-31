#pragma once
#include "Package.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
class GPS
{
	SoftwareSerial _ss;
	TinyGPSPlus _gps;
public:
	GPS();
	void init();
	bool GetGPSData(Package& package);
	bool GetGPSData(String & data);
	bool GetGPSData(double& longitude, double& latitude, uint32_t& time);
	bool GetGPSData(char* longitude, char* latitude, char* time);
	~GPS();
};