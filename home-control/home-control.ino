#include <Bounce2.h>

#include <autoHouse.h>

#define ST_CP  7
#define SH_CP 6
#define DS    8
#define DP1   9
#define DP2   10
#define DP3   11
#define DP4   12
#define B1P   A5
#define B2P   A4
#define B3P   A3
#define B4P   A2
#define TMP   A1
#define LP    A0
#define FP    3
#define LCP   5

automatedHouse smart = automatedHouse( ST_CP,  SH_CP,  DS,  DP1,  DP2,  DP3,  DP4,  B1P,  B2P,  B3P,  B4P,  TMP,  LP,  FP,  LCP);
  
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT );
  
  smart.setup();
}

void loop() {
  if(smart.buttonRead(2)) smart.enterMenu();
  if(smart.buttonRead(3)) smart.closeMenu();
  if(smart.buttonRead(0)) smart.SetPrevMenu();
  if(smart.buttonRead(1)) smart.SetNextMenu();
  smart.Show();

  //Serial.println(analogRead(LP));
  //Serial.println(smart.readTemp());
  smart.loop();
  
}
