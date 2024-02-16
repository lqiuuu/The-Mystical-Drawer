// void LEDsetup (){
//    Serial.println(F("Adafruit PID 5296 I2C QT 4x LED Arcade Buttons test!"));
  
//   if (!ss.begin(DEFAULT_I2C_ADDR)) {
//     Serial.println(F("seesaw not found!"));
//     while(1) delay(10);
//   }

//   uint16_t pid;
//   uint8_t year, mon, day;
  
//   ss.getProdDatecode(&pid, &year, &mon, &day);
//   Serial.print("seesaw found PID: ");
//   Serial.print(pid);
//   Serial.print(" datecode: ");
//   Serial.print(2000+year); Serial.print("/"); 
//   Serial.print(mon); Serial.print("/"); 
//   Serial.println(day);

//   if (pid != 5296) {
//     Serial.println(F("Wrong seesaw PID"));
//     while (1) delay(10);
//   }

//   Serial.println(F("seesaw started OK!"));
//   ss.pinMode(SWITCH1, INPUT_PULLUP);
//   ss.pinMode(SWITCH2, INPUT_PULLUP);
//   ss.pinMode(SWITCH3, INPUT_PULLUP);
//   ss.pinMode(SWITCH4, INPUT_PULLUP);
//   ss.analogWrite(PWM1, 127);
//   ss.analogWrite(PWM2, 127);
//   ss.analogWrite(PWM3, 127);
//   ss.analogWrite(PWM4, 127);
// }

void lightup(){
  for (int i = 0; i < 6; ++i) {
    switch (arr[i]) {
  case 1:
    ss.analogWrite(PWM1, 127);
    delay(1000);
    ss.analogWrite(PWM1, 0);
    delay(1000);
    break;
  case 2:
    ss.analogWrite(PWM2, 127);
    delay(1000);
    ss.analogWrite(PWM2, 0);
    delay(1000);
    break;
  case 3:
    ss.analogWrite(PWM3, 127);
    delay(1000);
    ss.analogWrite(PWM3, 0);
    delay(1000);
    break;
  case 4:
    ss.analogWrite(PWM4, 127);
    delay(1000);
    ss.analogWrite(PWM4, 0);
    delay(1000);
    break;
    }
  }
}

void LEDpuzzle(){
  if (ss.digitalRead(SWITCH1) != release1){
      if (! ss.digitalRead(SWITCH1)) { // SWITCH1 = 0
    Serial.println("Switch 1 pressed");
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
    Serial.println("Switch 2 pressed");
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
    Serial.println("Switch 3 pressed");
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
    Serial.println("Switch 4 pressed");
    ss.analogWrite(PWM4, 180);
    barr[count] = 4;
    count++;
  } else {
    ss.analogWrite(PWM4, 0);
  }
  release4 = ss.digitalRead(SWITCH4);
  }

if (count > 5){
  count = 0;
}
}

bool areArraysIdentical(const int array1[], const int array2[], int size) {
  // Check if the size of the arrays is the same
  if (size != sizeof(array1) / sizeof(array1[0]) || size != sizeof(array2) / sizeof(array2[0])) {
    return false;
  }

  // Compare corresponding elements of both arrays
  for (int i = 0; i < size; ++i) {
    if (array1[i] != array2[i]) {
      return false; // Arrays are not identical
    }
  }

  return true; // Arrays are identical
}

void compareLED (){

    for (int j = 0; j < 6; ++j){
      if (barr[j] != 0){
      if (arr[j] == barr[j]){
        digitalWrite(5, HIGH);
      }
    }
    }

}