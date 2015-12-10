#ifndef AH_H
#define AH_H

#include "Bounce2.h"
#include <Arduino.h>

#define IRpin_PIN PIND
#define MAXPULSE 10000 
#define RESOLUTION 20
#define FUZZINESS 20

class automatedHouse
{
private:

	/*Variables*/
	int latchPin;  //Pin connected to ST_CP of 74HC595 (aka RCLK)
	int clockPin;  //Pin connected to SH_CP of 74HC595 (aka SRCLK)
	int dataPin;  //Pin connected to DS of 74HC595 (aka SER)
	int buttonPins[4];
	int fanPin;
	int tempPin;
	int lightPin;
	int lightControlPin;
	int lightSensitivity = 127;
	int digit1Pin;  
	int digit2Pin;
	int digit3Pin;
	int digit4Pin;
	int digitPins[4];
	int temperatura = 15;
	int fanSpeed = 0;
	int lightLevels = 0;
	int fanSpeedPrev = 255;
	int lightLevelsPrev = 0;
	int subMenu[10];
	int mainMenuLast = 0;
	int subMenuLast[10];
	int lastMenuItem = 0;
	int currentMenuItem = 0;

	const int F = 10;
	const int C = 11;
	const int T = 12;
	const int E = 13;
	const int M = 14;
	const int P = 15;
	const int A = 16;
	const int DOT = 17;
	const int L = 18;

	unsigned int hold_level;
	unsigned int hold_refresh;

	unsigned long hold_timer;
	unsigned long refresh_timer;
	unsigned long prevRemoteButton = 0;

	uint8_t dataArray[19];
	uint16_t pulses[100][2];
	uint8_t currentpulse = 0;

	bool systemState = true;
	bool previous[4] = {LOW, LOW, LOW, LOW};
	bool fanAutoMode = true;
	bool lightAutoMode = true;
	bool fanAutoModePrev = false;
	bool lightAutoModePrev = false;
	bool mainMenuShow = false;
	bool subMenuShow = false;
	bool listemForIRloop = false;

	char* mainMenu[10];

	 

	Bounce buttonBounce[4] = {Bounce(), Bounce(), Bounce(), Bounce()};

	enum
	{
		temp,
		light,
		fan,
		lightAuto,
		fanAuto,
		lightSens
	};

	/*Functions*/
	int AddMenuItem(char string[]);

	void shiftOut(int myDataPin, int myClockPin, byte myDataOut);
	void setDigit(int digitPin, int value);
	void calculateLightSettings();
	void calculateFanSettings();
	void displayNumber(int number);
	void displayChar(int digit, const char character);
	void showSubmenu(int level);
	void setState(bool state);


public:		
	automatedHouse(int ST_CP, int SH_CP, int DS, int DP1, int DP2, int DP3, int DP4, int B1P, int B2P, int B3P, int B4P, int TMP, int LP, int FP, int LCP);
	~automatedHouse();		

	void setup();
	void ShowMenu(int level);
	void SetPrevMenu();
	void SetNextMenu();
	void enterMenu();
	void closeMenu();
	void Show();
	void loop();
	void SetLoop(bool listen);
	void IRaction(unsigned long result);

	bool buttonRead(int button);
	bool GetSystemState();

	int readTemp();
	int readLS();
	int GetCurrentMenuIndex();
};


#endif