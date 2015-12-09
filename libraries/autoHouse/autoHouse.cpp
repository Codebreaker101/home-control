#include <autoHouse.h>


automatedHouse::automatedHouse(int ST_CP, int SH_CP, int DS, int DP1, int DP2, int DP3, int DP4, int B1P, int B2P, int B3P, int B4P, int TMP, int LP, int FP, int LCP)
{
	latchPin = ST_CP; 
	clockPin = SH_CP; 
	dataPin = DS; 
	digitPins[0] = DP1;
	digitPins[1] = DP2;
	digitPins[2] = DP3;
	digitPins[3] = DP4;
	buttonPins[0] = B1P;
	buttonPins[1] = B2P;
	buttonPins[2] = B3P;
	buttonPins[3] = B4P;
    tempPin = TMP;
    lightPin = LP;
	fanPin = FP;
	lightControlPin = LCP;
}

automatedHouse::~automatedHouse()
{

}

void automatedHouse::setup()
{
	for(int i=0; i<4; i++)
	{
		pinMode(buttonPins[i],INPUT);
		digitalWrite(buttonPins[i], HIGH);
	    buttonBounce[i].attach(buttonPins[i]);
	    buttonBounce[i].interval(5);

	    pinMode(digitPins[i], OUTPUT);	
	}
	
    pinMode(tempPin, INPUT);
    pinMode(lightPin, INPUT);
	pinMode(latchPin, OUTPUT);
	pinMode(fanPin, OUTPUT);
	pinMode(lightControlPin, OUTPUT);

	dataArray[0] = B00111111;
	dataArray[1] = B00000110;
	dataArray[2] = B01011011;
	dataArray[3] = B01001111;
	dataArray[4] = B01100110;
	dataArray[5] = B01101101;
	dataArray[6] = B01111101;
	dataArray[7] = B00000111;
	dataArray[8] = B01111111;
	dataArray[9] = B01101111;
	dataArray[F] = B01110001;
	dataArray[C] = B00111001;
	dataArray[T] = B00110001;
	dataArray[E] = B01111001;
	dataArray[M] = B00110111;
	dataArray[P] = B01110011;
	dataArray[A] = B01110111;
	dataArray[DOT] = B10000000;
	dataArray[L] = B00111000;

	AddMenuItem("C   ");
	AddMenuItem("L   ");
	AddMenuItem("F   ");
	AddMenuItem("LA  ");
	AddMenuItem("FA  ");
	AddMenuItem("L5  ");

	hold_timer = millis();
	refresh_timer = millis();
	hold_level = 1000; 
	hold_refresh = 10;
	pinMode(13, OUTPUT);
}

bool automatedHouse::buttonRead(int button)
{
	buttonBounce[button].update();
	if(buttonBounce[button].read())
	{
		if (previous[button]) 
		{
			if (millis() > hold_timer+hold_level) 
			{
				if (millis() > refresh_timer+hold_refresh) 
				{
					refresh_timer = millis();
					return HIGH;
				}
				else return LOW;
			}
			else return LOW;
		}
		else 
		{
			previous[button] = true;
			hold_timer = millis();
			refresh_timer = millis();
			return HIGH;
		}
	}
	else
	{
		previous[button] = false;
		return LOW;
	}
}



int automatedHouse::readTemp() { return ((5.0 * analogRead(tempPin) * 100.0) / 1024); }

int automatedHouse::readLS() { return analogRead(lightPin); }

void automatedHouse::shiftOut(int myDataPin, int myClockPin, byte myDataOut) 
{
	int i=0;
	int pinState;
	pinMode(myClockPin, OUTPUT);
	pinMode(myDataPin, OUTPUT);

	digitalWrite(myDataPin, 0);
	digitalWrite(myClockPin, 0);
	for (i=7; i>=0; i--)  
	{
		digitalWrite(myClockPin, 0);
		if ( myDataOut & (1<<i) ) 
			pinState= 1;
		else 
			pinState= 0;
		digitalWrite(myDataPin, pinState);
		digitalWrite(myClockPin, 1);
		digitalWrite(myDataPin, 0);
	}
	digitalWrite(myClockPin, 0);
}

void automatedHouse::setDigit(int digitPin, int value)
{
	digitalWrite(latchPin, 0);
	shiftOut(dataPin, clockPin, dataArray[value]);  
	digitalWrite(latchPin, 1);
	digitalWrite(digitPin, HIGH);
	delay(1);
	digitalWrite(digitPin, LOW);   
}



void automatedHouse::displayNumber(int number)
{
	int temp;
	int thousand = number/1000;
	temp = number%1000;
	int hundred = temp/100;
	temp = temp%100;
	int ten = temp/10 ;
	temp = temp%10;
	int unit = temp;
  
	setDigit(digitPins[0], unit);
	if(number >= 10) setDigit(digitPins[1], ten);
	if(number >= 100) setDigit(digitPins[2], hundred);
	if(number >= 1000) setDigit(digitPins[3], thousand);
}

void automatedHouse::displayChar(int digit, const char character)
{

	switch(character)
	{
		case 'F':
		case 'f':
			setDigit(digitPins[digit], F);
			break;
		case 'C':
		case 'c':
			setDigit(digitPins[digit], C);
			break;
		case 'T':
		case 't':
			setDigit(digitPins[digit], T);
			break;
		case 'E':
		case 'e':
			setDigit(digitPins[digit], E);
			break;
		case 'M':
		case 'm':
			setDigit(digitPins[digit], M);
			break;
		case 'P':
		case 'p':
			setDigit(digitPins[digit], P);
			break;
		case 'A':
		case 'a':
			setDigit(digitPins[digit], A);
			break;
		case 'L':
		case 'l':
			setDigit(digitPins[digit], L);
			break;
		case '.':
			setDigit(digitPins[digit], DOT);
			break;
		case '0' ... '9':
			setDigit(digitPins[digit], (character-'0'));
			break;
		default:
			return;
	}
}	

int automatedHouse::AddMenuItem(char string[])
{
	mainMenu[mainMenuLast] = string;
	return mainMenuLast++;
}

void automatedHouse::ShowMenu(int level)
{
	for(int i=0, j=3; i<4, j>=0; i++, j--)
	{
		displayChar(i, mainMenu[level][j]);
	}
}

void automatedHouse::showSubmenu(int level)
{
	if(level == temp) displayNumber(temperatura);
	else if(level == light) displayNumber(255-lightLevels);
	else if(level == fan) displayNumber(255-fanSpeed);
	else if(level == lightAuto) displayNumber(lightAutoMode);
	else if(level == fanAuto) displayNumber(fanAutoMode);
	else if(level == lightSens) displayNumber(lightSensitivity);
	
}

void automatedHouse::SetPrevMenu()
{
	if(subMenuShow)
	{
		if(currentMenuItem == temp && temperatura > 0) temperatura--;	
		else if(currentMenuItem == fan && fanSpeed > 0) fanSpeed--;	
		else if(currentMenuItem == light && lightLevels > 0) lightLevels--;
		else if(currentMenuItem == lightAuto) lightAutoMode = !lightAutoMode;	
		else if(currentMenuItem == fanAuto) fanAutoMode = !fanAutoMode ;
		else if(currentMenuItem == lightSens && lightSensitivity > 0) lightSensitivity--; 	
	} 
	else if(currentMenuItem > 0)
	{
		lastMenuItem = currentMenuItem;
		currentMenuItem--; 	
	} 
}

void automatedHouse::SetNextMenu()
{
	if(subMenuShow)
	{
		if(currentMenuItem == temp && temperatura <= 49) temperatura++;
		else if(currentMenuItem == fan && fanSpeed <= 254) fanSpeed++;
		else if(currentMenuItem == light && lightLevels <= 254) lightLevels++;
		else if(currentMenuItem == lightAuto) lightAutoMode = !lightAutoMode;
		else if(currentMenuItem == fanAuto) fanAutoMode = !fanAutoMode;
		else if(currentMenuItem == lightSens && lightSensitivity <= 254) lightSensitivity++;
	}
	else if((currentMenuItem < mainMenuLast-1)) 
	{
		lastMenuItem = currentMenuItem;
		currentMenuItem++;
	}
}

int automatedHouse::GetCurrentMenuIndex()
{
	return currentMenuItem;
}

void automatedHouse::enterMenu()
{
	if(!GetSystemState()) setState(true);
	else if(!mainMenuShow && !subMenuShow)
	{
		mainMenuShow = true;
		subMenuShow = false;
	}
	else if(mainMenuShow && !subMenuShow)
	{
		mainMenuShow = false;
		subMenuShow = true;
	}
}

void automatedHouse::closeMenu()
{
	if(mainMenuShow) mainMenuShow = false;
	else if(subMenuShow)
	{
		mainMenuShow = true;
		subMenuShow = false;
	}
	else setState(false);
}

void automatedHouse::Show()
{
	if(subMenuShow) showSubmenu(currentMenuItem);
	if(mainMenuShow) ShowMenu(currentMenuItem);
}

void automatedHouse::loop()
{
	if(!fanAutoMode) analogWrite(fanPin, 255-fanSpeed);
	else if(fanAutoMode) calculateFanSettings();
	if(!lightAutoMode) analogWrite(lightControlPin, 255-lightLevels);
	else if(lightAutoMode) calculateLightSettings();
	
}

void automatedHouse::setState(bool state)
{
	if(state)
	{
		fanSpeed = fanSpeedPrev;
		lightLevels = lightLevelsPrev;
		fanAutoMode = fanAutoModePrev;
		lightAutoMode = lightAutoModePrev;
		mainMenuShow = false;
		subMenuShow = false;
		systemState = true;
		analogWrite(lightControlPin, 255-lightLevels);
		analogWrite(fanPin, 255-fanSpeed);
		digitalWrite(13, HIGH);
	}
	else
	{
		
		fanAutoModePrev = fanAutoMode;
		fanAutoMode = 0;
		lightAutoModePrev = lightAutoMode;
		lightAutoMode = 0;
		fanSpeedPrev = fanSpeed;
		fanSpeed = 255;
		lightLevelsPrev = lightLevels;
		lightLevels = 255;
		systemState = false;
		digitalWrite(13, LOW);
	}
	loop();
}

bool automatedHouse::GetSystemState()
{
	return systemState;
}

void automatedHouse::IRaction(unsigned long result)
{
	switch(result)
	{
		case 16753245: //On/OFF
			if(systemState) setState(false);
			else setState(true);
			//prevRemoteButton=16753245;
			break;
		case 16754775: // - sign
			if(lightLevels != 255) lightLevels+=15;
			prevRemoteButton = 16754775;
			break;
		case 16748655: // + sign
			if(lightLevels != 0) lightLevels-=15;
			prevRemoteButton = 16748655;
			break;
		case 16720605: // play/pause
			fanAutoMode = !fanAutoMode;
			//prevRemoteButton = 16720605;
			break;
		case 16769055: // EQ
			lightAutoMode = !lightAutoMode;
			//prevRemoteButton = 16769055;
			break;
		case 16712445: //prev
			if(fanSpeed != 255) fanSpeed+=15;
			prevRemoteButton = 16712445;
			break;
		case 553536955:
			if(fanSpeed != 0) fanSpeed -=15;
			prevRemoteButton = 553536955;
			break;
		case 4294967295: // hold
			IRaction(prevRemoteButton);
			break;
		default:
			break;
	}
	delay(50);
}

void automatedHouse::calculateLightSettings()
{
	int tmp = analogRead(lightPin);
	if(tmp < (650-lightSensitivity) && lightLevels >0)
	{
		lightLevels--;
	}
	if(tmp > (650+lightSensitivity) && lightLevels <255)
	{
		lightLevels++;
	}
	analogWrite(lightControlPin, 255-lightLevels);
}

void automatedHouse::calculateFanSettings()
{
	int tmp = readTemp();
	/*Serial.print("Trenutna: ");
	Serial.println(tmp);
	Serial.print("Zeljena: ");
	Serial.println(temperatura);*/
	if(tmp > temperatura)
	{
		fanSpeed = 0;
	}
	else fanSpeed = 255;

	analogWrite(fanPin, 255 - fanSpeed);
}
