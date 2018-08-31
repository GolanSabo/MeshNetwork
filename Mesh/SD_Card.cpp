//#include "SD_Card.h"
//
//
//const int CS_PIN = 4;
//
//SD_Card::SD_Card()
//{
//}
////int counter = 0;
//void SD_Card::initializeSD()
//{
//	Serial.println("Initializing SD card...");
//	pinMode(CS_PIN, OUTPUT);
//
//	if (SD.begin())
//	{
//		Serial.println("SD card is ready to use.");
//	}
//	else
//	{
//		Serial.println("SD card initialization failed");
//		return;
//	}
//}
//
//bool SD_Card::createFile(String filename)
//{
//	//Serial.println(filename);
//	file = SD.open("Golan.txt", FILE_WRITE);
//	if (file)
//	{
//		Serial.println("File created successfully.");
//		file.close();
//		return 1;
//	}
//	else
//	{
//		Serial.println("Error while creating file.");
//		return 0;
//	}
//}
//
//int SD_Card::writeToFile(char text[])
//{
//	if (file)
//	{
//		file.println(text);
//		Serial.println("Writing to file: ");
//		Serial.println(text);
//		return 1;
//	}
//	else
//	{
//		Serial.println("Couldn't write to file");
//		return 0;
//	}
//}
//
//void SD_Card::closeFile()
//{
//	if (file)
//	{
//		file.close();
//		Serial.println("File closed");
//	}
//}
//
//int SD_Card::openFile(char filename[])
//{
//	file = SD.open(filename);
//	if (file)
//	{
//		Serial.println("File opened with success!");
//		return 1;
//	}
//	else
//	{
//		Serial.println("Error opening file...");
//		return 0;
//	}
//}
//
//String SD_Card::readLine()
//{
//	String received = "";
//	char ch;
//	while (file.available())
//	{
//		ch = file.read();
//		if (ch == '\n')
//		{
//			return String(received);
//		}
//		else
//		{
//			received += ch;
//		}
//	}
//	return "";
//}
//
//SD_Card::~SD_Card()
//{
//}