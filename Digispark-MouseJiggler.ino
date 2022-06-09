/*

Mouse Jiggler
  
Hardware Requirements:
    Digispark (tested with Micronucleus 2.6 bootloader)

Library requirements (versions tested):
    Arduino IDE 1.8.19
    DigistumpArduino 1.7.5 (https://github.com/ArminJo/DigistumpArduino) 

 */

#include <DigiMouse.h>

char MAX = 40;            // Maximum amount of mouse movement
unsigned long onTime;     // Length of time for mouse action
unsigned long offTime;    // Amount of time to wait between mouse actions
bool currentlyOn;         // Keeps track of whether mouse action is happening
unsigned long startTime;  // For keeping track of millis timer

void setup() {

  startTime = millis();         // Initialize timer
  randomSeed(analogRead(2));    // Seed random number generator with floating analogue pin
  newTimes();                   // Generate new random times
  delay(100);                   // Short delay before initialising USB
  DigiMouse.begin();            // Initialise USB mouse
  square();                     // Move mouse cursor in a square (bootup test)
}

void loop() {
  //DigiMouse.update();
  if (currentlyOn && millis() > startTime + onTime) { // Stop for offTime seconds
    digitalWrite(1, LOW);
    currentlyOn = false;
    startTime = millis(); // Reset timer
    DigiMouse.update();
    newTimes();
  }
  if (!currentlyOn && millis() > startTime + offTime) { // Jiggle for onTime seconds
    DigiMouse.update();
    startTime = millis();
    digitalWrite(1, HIGH);

    //Generate a random number between 0 and 2. If it is greater than 0 call jiggle(), otherwise call scroll()
    char mode = random(0, 3);
    if (mode > 0) {
      while (millis() < startTime + onTime) {
        jiggle();
      }
    } else {
      while (millis() < startTime + onTime) {
        scroll();
      }
    }
    currentlyOn = true;
    startTime = millis(); // Reset timer
  }
}

void newTimes() {
  onTime = (unsigned long)random(100, 1000);      //How long mouse actions should be
  offTime = (unsigned long)random(8000, 30000);   //How long between actions
  DigiMouse.update();
}

void jiggle() {
  char dx = random(MAX + 1);
  char dy = random(MAX + 1);
  char count = random(10);

  while (count-- > 0) {
    DigiMouse.move((MAX >> 1) - dx,
                   (MAX >> 1) - dy,
                   0);
    DigiMouse.delay(random(1, 20));
    DigiMouse.update();
  }
}

void flash() {
  digitalWrite(1, HIGH);
  DigiMouse.delay(50);
  digitalWrite(1, LOW);
  DigiMouse.delay(100);

}

void scroll() {
  DigiMouse.scroll(random(-3, 3));
  DigiMouse.delay(60);
  DigiMouse.scroll(random(-3, 3));
  DigiMouse.delay(60);
}


void square() { //Moves the cursor in a square pattern - useful at startup to test USB connectivity

  for (int i = 0; i < 1000; i++) {
    DigiMouse.moveX(3);
    DigiMouse.delay(1);
  }

  flash();

  for (int i = 0; i < 1000; i++) {
    DigiMouse.moveY(3);
    DigiMouse.delay(1);
  }

  flash();

  for (int i = 0; i < 1000; i++) {
    DigiMouse.moveX(-3);
    DigiMouse.delay(1);
  }
  
  flash();
  
  for (int i = 0; i < 1000; i++) {
    DigiMouse.moveY(-3);
    DigiMouse.delay(1);
  }
  
  flash();
}
