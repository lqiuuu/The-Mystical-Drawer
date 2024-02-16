//1 = LED sequence
//2 = LED press button
//3 = piece puzzle
//4 = RFID lock


#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
// #include "voids.h"

#define  DEFAULT_I2C_ADDR 0x3A

#define  SWITCH1  18  // PA01
#define  SWITCH2  19 // PA02
#define  SWITCH3  20 // PA03
#define  SWITCH4  2 // PA06
#define  PWM1  12  // PC00
#define  PWM2  13 // PC01
#define  PWM3  0 // PA04
#define  PWM4  1 // PA05

Adafruit_seesaw ss;

//LED
int arr[6];
int barr[6];

int release1 = 1;
int release2 = 1;
int release3 = 1;
int release4 = 1;

int count = 0;

//RFID
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// byte nuidPICC[4];
// byte cardID[4] = {0x53, 0xDD, 0x2D, 0xF8};
byte nuidPICC[7];
byte cardID[7] = {0x1D, 0x3C, 0x99, 0xDC, 0x94, 0x00, 0x00};

//gamestages
int currentStage = 1;
//ledLight, ledSolve, unlock1, magnetPuzzle, unlock2, rfidRead, unlock3
// void stage1();
// void stage2();
// void stage3();
// void stage4();
// void stage5();

int sole1 = 2;
int sole2 = 3;
int sole3 = 4;

int reed1 = 5;
int reed2 = 6;
int reed3 = 7;
int reed4 = 8;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  for (int i = 0; i < 6; ++i){
   arr[i] = rand() % 4 + 1;
  }
  // LEDsetup();
   Serial.println(F("Adafruit PID 5296 I2C QT 4x LED Arcade Buttons test!"));
  
  if (!ss.begin(DEFAULT_I2C_ADDR)) {
    Serial.println(F("seesaw not found!"));
    while(1) delay(10);
  }

  uint16_t pid;
  uint8_t year, mon, day;
  
  ss.getProdDatecode(&pid, &year, &mon, &day);
  Serial.print("seesaw found PID: ");
  Serial.print(pid);
  Serial.print(" datecode: ");
  Serial.print(2000+year); Serial.print("/"); 
  Serial.print(mon); Serial.print("/"); 
  Serial.println(day);

  if (pid != 5296) {
    Serial.println(F("Wrong seesaw PID"));
    while (1) delay(10);
  }

  Serial.println(F("seesaw started OK!"));
  ss.pinMode(SWITCH1, INPUT_PULLUP);
  ss.pinMode(SWITCH2, INPUT_PULLUP);
  ss.pinMode(SWITCH3, INPUT_PULLUP);
  ss.pinMode(SWITCH4, INPUT_PULLUP);
  // ss.analogWrite(PWM1, 127);
  // ss.analogWrite(PWM2, 127);
  // ss.analogWrite(PWM3, 127);
  // ss.analogWrite(PWM4, 127);


  //RFID
  SPI.begin();                             // Init SPI bus
  mfrc522.PCD_Init();              // Init MFRC522
  delay(4);                                             // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();      // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  //pins
  pinMode(sole1, OUTPUT);
  pinMode(sole2, OUTPUT);
  pinMode(sole3, OUTPUT);

  pinMode(reed1, INPUT);
  pinMode(reed2, INPUT);
  pinMode(reed3, INPUT);
  pinMode(reed4, INPUT);

}

void loop() {
 switch (currentStage) {
    case 1:
      ledLight();
      break;

    case 2:
      ledSolve();
      break;

    case 3:
      unlock1();
      break;

    case 4:
      magnetPuzzle();
      break; 

    case 5:
      unlock2();
      break;

    case 6:
      rfidRead();
      break; 

    case 7:
      unlock3();
      break;   

    case 8:
      gameDone();
      break;    
     

    // Add more cases for additional stages as needed


}
}

//1
void ledLight(){
  // reset input array
   for (int i = 0; i < 6; ++i) {
    barr[i] = 0; // Reset each element to 0
  }
  // lightup();
  for (int i = 0; i < 6; ++i) {
    switch (arr[i]) { 
  case 1:
    ss.analogWrite(PWM1, 150);
    delay(1500);
    ss.analogWrite(PWM1, 0);
    delay(500);
    break;
  case 2:
    ss.analogWrite(PWM2, 127);
    delay(1500);
    ss.analogWrite(PWM2, 0);
    delay(500);
    break;
  case 3:
    ss.analogWrite(PWM3, 127);
    delay(1500);
    ss.analogWrite(PWM3, 0);
    delay(500);
    break;
  case 4:
    ss.analogWrite(PWM4, 150);
    delay(1500);
    ss.analogWrite(PWM4, 0);
    delay(500);
    break;
    }
  }
  currentStage = 2;
}

//2
void ledSolve(){
  // button input
  if (ss.digitalRead(SWITCH1) != release1){
      if (! ss.digitalRead(SWITCH1)) { // SWITCH1 = 0
    // Serial.println("Switch 1 pressed");
    ss.analogWrite(PWM1, 127);
    barr[count] = 1;
    count++;
  } else {
    ss.analogWrite(PWM1, 0);
  }
  release1 = ss.digitalRead(SWITCH1);
  }

  if (ss.digitalRead(SWITCH2) != release2){
    if (! ss.digitalRead(SWITCH2)) {
    // Serial.println("Switch 2 pressed");
    ss.analogWrite(PWM2, 127);
    barr[count] = 2;
    count++;
  } else {
    ss.analogWrite(PWM2, 0);
  }
  release2 = ss.digitalRead(SWITCH2);
  }
  
  if (ss.digitalRead(SWITCH3) != release3){
      if (! ss.digitalRead(SWITCH3)) {
    // Serial.println("Switch 3 pressed");
    ss.analogWrite(PWM3, 127);
    barr[count] = 3;
    count++;
  } else {
    ss.analogWrite(PWM3, 0);
  }
  release3 = ss.digitalRead(SWITCH3);
  }
  
  if (ss.digitalRead(SWITCH4) != release4){
      if (! ss.digitalRead(SWITCH4)) {
    // Serial.println("Switch 4 pressed");
    ss.analogWrite(PWM4, 180);
    barr[count] = 4;
    count++;
  } else {
    ss.analogWrite(PWM4, 0);
  }
  release4 = ss.digitalRead(SWITCH4);
  }

// if input wrong (> detect when exceed 6 inputs)
if (count > 6){
  count = 0;
    ss.analogWrite(PWM1, 0);
    ss.analogWrite(PWM2, 0);
    ss.analogWrite(PWM3, 0);
    ss.analogWrite(PWM4, 0);
  currentStage = 1;
}
  // compareLED();
  if (areArraysIdentical(arr, barr, 6)) {
    currentStage = 3;
    ss.analogWrite(PWM1, 0);
    ss.analogWrite(PWM2, 0);
    ss.analogWrite(PWM3, 0);
    ss.analogWrite(PWM4, 0);
  } else {
    digitalWrite(5, LOW);
  }

  //serial print
  for (int i = 0; i < 6; ++i) {
    Serial.print(arr[i]);
    Serial.print(" ");
    Serial.print(barr[i]);
    Serial.print(",");
  }
  Serial.println();
    
}

// array comparison code
bool areArraysIdentical(const int array1[], const int array2[], int size) {
  // Compare corresponding elements of both arrays
  for (int i = 0; i < size; ++i) {
    if (array1[i] == 0 || array2[i] == 0) {
      return false;
      }

    if (array1[i] != array2[i]) {
      return false; // Arrays are not identical
    } 

  }

  return true; // Arrays are identical
}

//3
void unlock1(){
  LEDCorrect();
  LEDCorrect();
  LEDCorrect();
  digitalWrite(sole1, HIGH);
    delay(15000);
  digitalWrite(sole1, LOW);
    // delay(5000)
  currentStage = 4;
}

//4
void magnetPuzzle(){
  // Serial.println("stage: " + currentStage);
  // Serial.println(digitalRead(6));
  if (digitalRead(reed1)== LOW && digitalRead(reed2)== LOW && digitalRead(reed3) == LOW && digitalRead(reed1) == LOW){
    currentStage = 5;
  }else{
    digitalWrite(sole2, LOW);
  }

}

//5
void unlock2(){
  digitalWrite(sole2, HIGH);
    delay(15000);
  digitalWrite(sole2, LOW);
    // delay(5000)
  currentStage = 6;
}

//6
void rfidRead(){
   // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
            if ( ! mfrc522.PICC_IsNewCardPresent()) {
                        return;
            }
            // Select one of the cards
            if ( ! mfrc522.PICC_ReadCardSerial()) {
                        return;
            }

            for(byte i = 0; i < 4; i ++){
              nuidPICC[i] = mfrc522.uid.uidByte[i];
            }
            
            if (nuidPICC[0] == cardID[0] && nuidPICC[1] == cardID[1] 
              && nuidPICC[2] == cardID[2] && nuidPICC[3] == cardID[3]){
                currentStage = 7;
            }

            // Dump debug info about the card; PICC_HaltA() is automatically called
            mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

//7
void unlock3(){
  digitalWrite(sole3, HIGH);
    delay(15000);
  digitalWrite(sole3, LOW);
    // delay(5000)
  currentStage = 8;
}

//8
void gameDone() {
  // Serial.println("stage: " + currentStage);
  delay(5000);
  digitalWrite(5, LOW);
}

void LEDCorrect(){
  ss.analogWrite(PWM1, 150);
  delay(50);
  ss.analogWrite(PWM1, 0);
  ss.analogWrite(PWM2, 127);
  delay(50);
  ss.analogWrite(PWM2, 0);
  ss.analogWrite(PWM3, 127);
  delay(50);
  ss.analogWrite(PWM3, 0);
  ss.analogWrite(PWM4, 160);
  delay(50);
  ss.analogWrite(PWM4, 0);
}
