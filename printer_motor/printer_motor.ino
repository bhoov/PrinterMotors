
/**** Object oriented approach working *****/
// include the library code:
#include <Wire.h>
#include <AFMotor.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <printerMotor.h>


// Initilialize LCD screen
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Define backlight colors for LCD shield
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// Define globals  for Menus
#define MANUAL_MODE 0
#define MEMORY_MODE 1
int menuMode;

// Define switch actives
bool upActive;
bool downActive;
bool leftActive;
bool rightActive;

// Initialize motors
PrinterMotor motorLeft(14, 1, 2); // Potentiometer pin on A14. Use motors 1 & 2
PrinterMotor motorRight(15, 3, 4); // Potentiometer pin on A15. Use motors 3 & 4

/************* Begin Code **************/
void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setBacklight(GREEN);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Manual Mode");
  menuMode = MANUAL_MODE;

  // Initialize states
  upActive = false;
  downActive = false;
  leftActive = false;
  rightActive = false;

}

void loop() {
  // Check if button has been pressed, forward to command accordingly
  checkButtons();

  // Update potentiometer on both motors
  motorRight.updatePot();
  motorLeft.updatePot();

  // Check limits

  printMotorPot();
}

void checkButtons() {
  // Button functionality
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    if (buttons & BUTTON_UP) {
      switch (menuMode) {
        case MANUAL_MODE:
          buttonUp();
          break;
        case MEMORY_MODE:
          break;
      }
    }
    if (buttons & BUTTON_DOWN) {
      switch (menuMode) {
        case MANUAL_MODE:
          buttonDown();
          break;
        case MEMORY_MODE:
          break;
      }

    }
    if (buttons & BUTTON_LEFT) {
      switch (menuMode) {
        case MANUAL_MODE:
          buttonLeft();
          break;
        case MEMORY_MODE:
          break;
      }
    }
    if (buttons & BUTTON_RIGHT) {
      switch (menuMode) {
        case MANUAL_MODE:
          buttonRight();
          break;
        case MEMORY_MODE:
          motorRight.goTo(10600);
          break;
      }
    }
    if (buttons & BUTTON_SELECT) {
      buttonSelect();
    }
    delay(100); // Make better debouncing
  }
}

void buttonUp() {
  // Functionality when up button is pressed in manual mode

  if (upActive) {
    upActive = false;
    lcd.clear();
    lcd.print("Stopping");
    motorRight.release();
    motorLeft.release();
  }

  // if no buttons are pressed:
  else if (!buttonActivated()) {
    upActive = true;
    lcd.clear();
    lcd.print("Going Up");

    motorRight.go(FORWARD);
    motorLeft.go(FORWARD);
  }
}

void buttonDown() {
  // Functionality when down button is pressed in manual mode

  // If already going down, stop
  if (downActive) {
    downActive = false;
    lcd.clear();
    lcd.print("Stopping");
    motorRight.release();
    motorLeft.release();
  }

  // If no buttons are pressed, go down
  else if (!buttonActivated()) {
    downActive = true;
    lcd.clear();
    lcd.print("Going Down");

    motorRight.go(BACKWARD);
    motorLeft.go(BACKWARD);
  }
}

void buttonRight() {
  // Functionality when down button is pressed in manual mode

  // If already going down, stop
  if (rightActive) {
    rightActive = false;
    lcd.clear();
    lcd.print("Stopping");
    motorRight.release();
  }

  // If no buttons are pressed, go down
  else if (!buttonActivated()) {
    rightActive = true;
    lcd.clear();
    lcd.print("Right Motor Up");
    motorRight.go(FORWARD);
  }
}

void buttonLeft() {
  // Functionality when down button is pressed in manual mode

  // If already going down, stop
  if (leftActive) {
    leftActive = false;
    lcd.clear();
    lcd.print("Stopping");
    motorLeft.release();
  }
  // If no buttons are pressed, go down
  else if (!buttonActivated()) {
    leftActive = true;
    lcd.clear();
    lcd.print("Left Motor Up");
    motorLeft.go(FORWARD);
  }
}

void buttonSelect() {
  if (!buttonActivated()) {
    // Change menu display and mode
    lcd.setBacklight(VIOLET);
    switch (menuMode) {
      case MANUAL_MODE: // go to Memory Mode
        lcd.setBacklight(VIOLET);
        lcd.clear();
        lcd.print("Memory Mode");
        menuMode = MEMORY_MODE;
        break;
      case MEMORY_MODE: // go to Manual Mode
        lcd.setBacklight(GREEN);
        lcd.clear();
        lcd.print("Manual Mode");
        menuMode = MANUAL_MODE;
        break;
    }
  }
}
void calibrate() {}

bool buttonActivated() {
  return (upActive | downActive | leftActive | rightActive);
}

void deactivateButtons() {
  upActive = false;
  downActive = false;
  leftActive = false;
  rightActive = false;
}

void printMotorPot() {
  Serial.print("Left: ");
  motorLeft.printPot();
  Serial.print("\t");
  Serial.print("Right: ");
  motorRight.printPot();
  Serial.println("");
}

void checkMotorLimits() {
  // If right limit, 
  if (motorRight.checkLimits()){

  }

  if (motorLeft.checkLimits()){
    
  }
}

