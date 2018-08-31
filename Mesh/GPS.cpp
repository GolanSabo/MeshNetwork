#include "GPS.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//-----------------configuration GPS------------------
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;


GPS::GPS() :_ss(RXPin, TXPin)
{
}

void GPS::init()
{
	_ss.begin(GPSBaud);
	
}

bool GPS::GetGPSData(Package& package) {
	bool status = false;
	if (_ss.available() > 0) {
		if (_gps.encode(_ss.read())) {
			String gpsData(_gps.location.lat()+ ',');
			String longitude(_gps.location.lng() + ',');
			String time(_gps.time.value());
			gpsData.concat(longitude);
			gpsData.concat(time);
			gpsData.toCharArray(package.data, gpsData.length(), 0);
			status = true;
		}
	}
	return status;
}

bool GPS::GetGPSData(String& gpsData) {
	bool status = false;
	while (_ss.available() > 0) {

		if (_gps.encode(_ss.read())) {
			String longitude(_gps.location.lng());
			String time(_gps.time.value());
			gpsData += _gps.location.lat();
			gpsData += ',';
			gpsData.concat(longitude);
			gpsData += ',';
			gpsData.concat(time);

			status = true;
		}

	}

	return status;
}

bool GPS::GetGPSData(double& longitude, double& latitude, uint32_t& time) {
	bool status = false;
	while (_ss.available() > 0) {

		if (_gps.encode(_ss.read())) {
			latitude = _gps.location.lat();
			longitude = _gps.location.lng();
			time = _gps.time.value();

			status = true;
		}

	}

	return status;
}

bool GPS::GetGPSData(char* longitude, char* latitude, char* time) {
	bool status = false;
	double lat = 0, lng = 0;
	uint32_t tm = 0;
	while (_ss.available() > 0) {

		if (_gps.encode(_ss.read())) {
			lat = _gps.location.lat();
			lng = _gps.location.lng();
			tm = _gps.time.value();
			sprintf(longitude, "%.1f", lng);
			sprintf(latitude, "%.1f", lat);
			sprintf(time, "%d", tm);
			status = true;
		}

	}

	return status;
}
GPS::~GPS()
{
}
