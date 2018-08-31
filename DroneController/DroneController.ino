#pragma once
#include <Wire.h>
//this programm will put out a PPM signal

//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String commandString = "";
String channelString = "4";
boolean flag = false;


/*this array holds the servo values for the ppm signal
change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[chanel_number];

//Red wire - 5V
//Yellow wire - 10
//Brown wire - ground
void setup() {
	Wire.begin(8);                // join i2c bus with address #8
	Wire.onReceive(receiveEvent); // register event
	Serial.begin(115200);           // start serial for output
	
	//initiallize default ppm values
	for (int i = 0; i<chanel_number; i++) {
		ppm[i] = default_servo_value;
	}
	//ppm[0] = 1500;

	pinMode(sigPin, OUTPUT);
	digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)

	cli();
	TCCR1A = 0; // set entire TCCR1 register to 0
	TCCR1B = 0;

	OCR1A = 100;  // compare match register, change this
	TCCR1B |= (1 << WGM12);  // turn on CTC mode
	TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
	TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
	sei();

}

void loop() {
	//parseTest("41900");
	delay(500);
}

void receiveEvent(int howMany) {

	char command[5];
	while (0 < Wire.available()) { // loop through all but the last
		Wire.readBytes(command,5);

	}

	String str(command);
	int direction = str.substring(0, 1).toInt();
	Serial.print("direction = ");
	Serial.println(direction);
	int amount = str.substring(1, 5).toInt();
	Serial.print("amount = ");
	Serial.println(amount);
	if ((amount > 1000 && amount <= 1900) && (direction >= 0 && direction <= 8)) {

		ppm[direction] = amount;
	}
}

//void parseTest(String s) {
//	Serial.println("parseTest");
//	Serial.println(s);
//	String substring1 = s.substring(0, 1);
//	String substring2 = s.substring(1, 5);
//	Serial.println(substring1);
//	Serial.println(substring2);
//
//	int direction = s.substring(0, 1).toInt();
//	int amount = s.substring(1, 5).toInt();
//	Serial.print("amount = ");
//	Serial.println(amount);
//	Serial.print("direction = ");
//	Serial.println(direction);
//	if ((amount > 1000 && amount <= 1900) && (direction >= 0 && direction <= 8)) {
//
//		ppm[direction] = amount;
//	}
//}

ISR(TIMER1_COMPA_vect) {  //leave this alone
	static boolean state = true;

	TCNT1 = 0;

	if (state) {  //start pulse
		digitalWrite(sigPin, onState);
		OCR1A = PPM_PulseLen * 2;
		state = false;
	}
	else {  //end pulse and calculate when to start the next pulse
		static byte cur_chan_numb;
		static unsigned int calc_rest;

		digitalWrite(sigPin, !onState);
		state = true;

		if (cur_chan_numb >= chanel_number) {
			cur_chan_numb = 0;
			calc_rest = calc_rest + PPM_PulseLen;// 
			OCR1A = (PPM_FrLen - calc_rest) * 2;
			calc_rest = 0;
		}
		else {
			OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * 2;
			calc_rest = calc_rest + ppm[cur_chan_numb];
			cur_chan_numb++;
		}
	}
}
