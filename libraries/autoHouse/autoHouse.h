#ifndef AH_H
#define AH_H

#include "Bounce2.h"
#include <Arduino.h>

class automatedHouse
{
private:
	bool systemState = true;
	int latchPin;  //Pin connected to ST_CP of 74HC595 (aka RCLK)
	int clockPin;  //Pin connected to SH_CP of 74HC595 (aka SRCLK)
	int dataPin;  //Pin connected to DS of 74HC595 (aka SER)
	int buttonPins[4];
	bool previous[4] = {LOW, LOW, LOW, LOW};
	int fanPin;
	int tempPin;
	int lightPin;
	int lightControlPin;
	int lightSensitivity = 127;
	#define IRpin_PIN PIND

	#define MAXPULSE 10000 
	#define RESOLUTION 20
	#define FUZZINESS 20


	const int F = 10;
	const int C = 11;
	const int T = 12;
	const int E = 13;
	const int M = 14;
	const int P = 15;
	const int A = 16;
	const int DOT = 17;
	const int L = 18;

	uint8_t dataArray[19];

	Bounce buttonBounce[4] = {Bounce(), Bounce(), Bounce(), Bounce()};

	unsigned long hold_timer;
	unsigned long refresh_timer;
	unsigned int hold_level;
	unsigned int hold_refresh;

	uint16_t pulses[100][2];
	uint8_t currentpulse = 0; 

	void shiftOut(int myDataPin, int myClockPin, byte myDataOut);
	void setDigit(int digitPin, int value);

	int digit1Pin;  
	int digit2Pin;
	int digit3Pin;
	int digit4Pin;

	int digitPins[4];

	enum
	{
		temp,
		light,
		fan,
		lightAuto,
		fanAuto,
		lightSens
	};

	int temperatura = 15;
	int fanSpeed = 0;
	int lightLevels = 0;
	bool fanAutoMode = true;
	bool lightAutoMode = true;

	int fanSpeedPrev = 255;
	int lightLevelsPrev = 0;
	bool fanAutoModePrev = false;
	bool lightAutoModePrev = false;
	unsigned long prevRemoteButton = 0;

public:		
	automatedHouse(int ST_CP, int SH_CP, int DS, int DP1, int DP2, int DP3, int DP4, int B1P, int B2P, int B3P, int B4P, int TMP, int LP, int FP, int LCP);
	~automatedHouse();		

	void setup();
	bool buttonRead(int button);

	int readTemp();
	int readLS();

private:
	char* mainMenu[10];
	int subMenu[10];
	int mainMenuLast = 0;
	int subMenuLast[10];
	int AddMenuItem(char string[]);
	void calculateLightSettings();
	void calculateFanSettings();
	int lastMenuItem = 0;
	int currentMenuItem = 0;
	void displayNumber(int number);
	void displayChar(int digit, const char character);
	void showSubmenu(int level);

	bool mainMenuShow = false;
	bool subMenuShow = false;

	bool listemForIRloop = false;

	void setState(bool state);

public:
	void ShowMenu(int level);
	void SetPrevMenu();
	void SetNextMenu();
	void enterMenu();
	void closeMenu();
	void Show();
	void loop();
	int GetCurrentMenuIndex();
	void SetLoop(bool listen);
	void IRaction(unsigned long result);
	bool GetSystemState();
};


#endif