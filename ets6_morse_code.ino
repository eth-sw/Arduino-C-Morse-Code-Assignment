const int arrayLength = 48;
char characters[arrayLength] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', '\'', '?', '!', '+', '-', '*', '/', '=', ' '};
char * sacode[arrayLength] = {"+", "@@+", "@+>", "+@", "@", "@+@", "@++", "++", "@@", "+@@", "+>", "@@>", "@+", ">+", "@>", ">@", "+@>", "@>+", "@>@", ">", ">>", "+++", "++@", "+>@", "@>>", "++>", ">@@+", ">@@>", ">@+@", ">@++", ">@+>", ">@>@", ">@>+", ">@>>", ">+@@", ">@@@", ">@@", ">@+", ">+@", ">++", ">+>", ">>@", ">>+", ">>>", ">@>", ">+++>", " "};

byte ledPin; // variable for storing current led light in use

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String input = inputPrompt(""); // calls function to prompt the user to enter data
  inputSequence(input);
}

void inputSequence(String userInput) {
  if (userInput[0] == '@' || userInput[0] == '+' || userInput[0] == '>') { // identifies user is entering SACode
    if (userInput[1] == '>' && userInput[2] == '+' && userInput[3] == '/' && userInput[4] == '+' && userInput[5] == '/' && userInput[6] == '>') { // "RA" Command combination
      RAInput(userInput);
    }
    else if (userInput[1] == '>' && userInput[2] == '+' && userInput[3] == '/' && userInput[4] == '@' && userInput[5] == '@' && userInput[6] == '/' && userInput[7] == '@' && userInput[8] == '>' && userInput[9] == '+') { // "RIR" Command combination
      RIRCommand();
    }
    else if (userInput[1] == '/' && userInput[2] == '>' && userInput[3] == '@' && userInput[4] == '@') { // "A" Command
      int ANum1; // input must either be 0, 1, or 2
      if (userInput[5] == '@') {
        ANum1 = 0;
        ACommand(userInput, ANum1);
      }
      else if (userInput[5] == '+') {
        ANum1 = 1;
        ACommand(userInput, ANum1);
      }
      else if (userInput[5] == '>') {
        ANum1 = 2;
        ACommand(userInput, ANum1);
      }
    }
    else if (userInput[1] == '+' && userInput[2] == '+' && userInput[3] == '/' && userInput[4] == '@' && userInput[5] == '+' && userInput[6] == '@') { // "VF" Command
      VFCommand(userInput);
    }
    else { // if NO commands entered
      ledPin = 11; // BLUE pin
      String sacodePin = userInput;
      Serial.println(sacodeToAscii(userInput)); // sacodeToAscii function called passing userInput as parameter
      pinMode(ledPin, OUTPUT);
      String sacode = sacodePin;
      sendDigital(ledPin, sacode); // calls sendDigital for outputting to LED
    }
  }
  else {
    ledPin = 10; // GREEN pin
    Serial.println(asciiToSacode(userInput)); // asciiToSacode function called passing userInput as parameter
    pinMode(ledPin, OUTPUT);
    String sacode = asciiToSacode(userInput); // converts the ascii to sacode ready for output to LED
    sendDigital(ledPin, sacode);
  }
}

String inputPrompt(String input) { // called at start of loop so that code listens for next message to process
  Serial.print(input);
  while (!Serial.available()) {};
  return Serial.readString();
}

char * charToSacode (char charInput) { // converts individual characters to sacode using pointers in sacode array
  charInput = toupper(charInput); // converts all characters to uppercase
  for (int i = 0; i < arrayLength; i++) {
    if (charInput == characters[i]) {
      return sacode[i];
    }
    else if (charInput == '@' || charInput == '>') { // excludes '+' character as it is required for sacode
      return ('?'); // returns error character
    }
    switch (charInput) {
      case ' ':
        return (sacode[46]); // retrieves ' ' character from the sacode array
        break;
    }
  }
}

char sacodeToChar(String sacharInput) { // converts a each sacode character string back to individual ascii characters
  for (int i = 0; i < arrayLength; i++) {
    if (sacharInput == sacode[i]) {
      return (characters[i]);
    }
  }
  return ('?'); // returns error character
}

String asciiToSacode (String asciiInput) { // converts ascii string to sacode equivalent
  asciiInput.toUpperCase(); // converts all chars in ascii string to uppercase
  String sacode = "";

  int len = asciiInput.length();
  for (int i = 0; i < len; i++) {
    sacode = sacode + charToSacode(asciiInput[i]); // calls charToSacode to convert each ascii character into sacode equivalent // loops through string
    if (asciiInput[i] != ' ' && asciiInput[i + 1] != ' ' && i < len - 1) { // ensures '/' character is placed in correct location between characters forming the same string
      sacode += '/';
    }
  }
  return sacode;
}

String sacodeToAscii(String sacodeInput) { // converts sacode string to ascii equivalent
  int saIndex = 0;
  char saChar;
  String strInit; // passed as parameter to sacodeToChar
  String saConv; // initialises to an empty string
  while (saIndex < sacodeInput.length()) { // loops until
    saChar = sacodeInput[saIndex];
    switch (saChar) {
      case ' ' : // identifier for 2 separate words
        if (strInit.length()) {
          saConv += sacodeToChar(strInit);
          strInit = "";
        }
        saConv += ' ';
        break;
      case '/': // identifier for 2 separate characters
        if (strInit.length()) {
          saConv += sacodeToChar(strInit);
          strInit = "";
        }
        break;
      case '@': // identifiers for sacode characters
      case '>':
      case '+':
        strInit += saChar;
        if (strInit.length() > 8) {
          saConv += sacodeToChar(strInit); // calls sacodeToChar function to translate each sacode 'letter' into ascii // adds translated character to 'a' string
          strInit = "";
        }
        break;
    }
    saIndex++;
  }
  if (strInit.length()) {
    saConv += sacodeToChar(strInit);
  }
  return saConv; // returns final translated result
}

void sendDigital(int pinNo, String sacode) { // retrieves either blue or green LED depending on user input and sacode as parameters
  float sensorValue = analogRead(A0); // REF https://www.instructables.com/LEDS-With-a-Potentiometer
  float timeUnit;
  int len = sacode.length(); 
  if (sensorValue == 1023) { // max potentiometer value equates to 400ms time unit
    timeUnit = 400;
  }
  else if (sensorValue == 0) { // min potentiometer value equates to 20ms time unit
    timeUnit = 20;
  }
  else {
    timeUnit = (380 * (sensorValue / 1023)) + 20; // retrieves correct time unit value (between 20 and 400) equivalent to the sensor value
  }

  for (int i = 0; i < len; i++) {
    if (sacode[i] == '@') {
      digitalWrite(ledPin, HIGH);
      delay(timeUnit); // 1 time unit
      digitalWrite(ledPin, LOW);
      delay(timeUnit);
    }
    else if (sacode[i] == '>') {
      digitalWrite(ledPin, HIGH);
      delay(timeUnit * 4); // 4 time units
      digitalWrite(ledPin, LOW);
      delay(timeUnit);
    }
    else if (sacode[i] == '+') {
      digitalWrite(ledPin, HIGH);
      delay(timeUnit * 2); // 2 time units
      digitalWrite(ledPin, LOW);
      delay(timeUnit);
    }
    else if (sacode[i] == ' ') {
      delay(timeUnit * 6); // 6 time units
    }
    else if (sacode[i] == '/') {
      delay(timeUnit * 3); // 3 time units
    }
  }
}

int RAInput (String userInput) { // called to retrieve the 3 decimal digits
  int RANum;
  String RATotal;
  int counter;
  int i = 5; // index begins at position 5 of user input
  for (i; i < 20; counter++) {
    if (userInput[i] == '/' && userInput[i + 1] == '>') {
      if (userInput[i + 2] == '@') {
        if (userInput[i + 3] == '@') {
          if (userInput [i + 4] == '+') {
            RANum = 1;
          }
          else if (userInput[i + 4] == '>') {
            RANum = 2;
          }
          else if (userInput[i + 4] == '@') {
            RANum = 0;
          }
          else {
            RANum = false;
          }
        }
        else if (userInput[i + 3] == '+') {
          if (userInput [i + 4] == '@') {
            RANum = 3;
          }
          else if (userInput[i + 4] == '+') {
            RANum = 4;
          }
          else if (userInput[i + 4] == '>') {
            RANum = 5;
          }
          else {
            RANum = false;
          }
        }
        else if (userInput[i + 3] == '>') {
          if (userInput[i + 4] == '@') {
            RANum = 6;
          }
          else if (userInput[i + 4] == '+') {
            RANum = 7;
          }
          else if (userInput[i + 4] == '>')  {
            RANum = 8;
          }
          else {
            RANum = false;
          }
        }
      }
      else if (userInput[i + 2] == '+') {
        RANum = 9;
      }
      else {
        RANum = false;
      }
    }
    i += 5; // 5 is length of one sacode character
    RATotal += String(RANum); // adds each entered number to the total
  }
  RACommand(RATotal.toInt()); // converts the integer total to the RACommand function
}

void RACommand(int digits) { // RACommand function called, passes the 3 digits entered by the user
  ledPin = 9; // YELLOW led
  pinMode(ledPin, OUTPUT);
  float sensorValue = analogRead(A0); // reads value potentiometer is set to (between 0 and 1023)
  float digitUnit;
  if (digits > 255) { // max user input permitted is 255 // sets any entered values over this to 255
    digits = 255;
  }

  if (sensorValue == 1023) {
    digitUnit = digits;
  }
  else if (sensorValue == 0) {
    digitUnit = 0;
  }
  else { 
    digitUnit = digits * (sensorValue / 1023); // calculates equivalent of user input to the value the potentiometer is set to
  }
  analogWrite(ledPin, digitUnit); // turns on LED at set brightness
}

void RIRCommand() { // RIRCommand does not need to take any parameters
  int irPin = 2; // ir receiver on pin 2 // REF https://medium.com/illumination/arduino-with-infrared-sensor-48ad4415f320
  int state = 0;
  int threshold = 200; // threshold for deciding if value is HIGH or LOW

  pinMode(irPin, INPUT);
  pinMode(state, INPUT);
  digitalWrite(state, HIGH);

  int data = analogRead(irPin); // reads value of IR receiver

  if (data >= threshold) {
    Serial.println("HIGH"); // greater than threshold
  }
  else {
    Serial.println("LOW"); // smaller than threshold
  }
}

void ACommand(String userInput, int num) { // ACommand takes userInput (sacode string) and the num(first number entered by user)
  int ANum1;
  int ANum2;
  int ANum3;
  int counter;
  int i = 6; // begins at index location 6
  String strSum;
  String finNum;
  for (i; i < 60; counter++) {
    if (userInput[i] == '/' && userInput[i + 1] == '>') { // begins at finding second number as first number has already been sought
      if (userInput[i + 2] == '@') {
        if (userInput[i + 3] == '@') {
          if (userInput[i + 4] == '+') { // Value 1
            ANum2 = 1;
          }
          else if (userInput[i + 4] == '>') { // Value 2
            ANum2 = 2;
          }
          else if (userInput[i + 4] == '@') {
            ANum2 = 0;
          }
          else {
            ANum2 = false;
          }
        }
        else if (userInput[i + 3] == '+') {
          if (userInput[i + 4] == '@') {
            ANum2 = 3;
          }
          else if (userInput[i + 4] == '+') {
            ANum2 = 4;
          }
          else if (userInput[i + 4] == '>') {
            ANum2 = 5;
          }
          else {
            ANum2 = false;
          }
        }
        else if (userInput[i + 3] == '>') {
          if (userInput[i + 4] == '@') {
            ANum2 = 6;
          }
          else if (userInput[i + 4] == '+') {
            ANum2 = 7;
          }
          else if (userInput[i + 4] == '>') {
            ANum2 = 8;
          }
          else {
            ANum2 = false;
          }
        }
      }
      else if (userInput[i + 2] == '+') {
        if (userInput[i + 3] == '@' && userInput[i + 3] == '@') {
          ANum2 = 9;
        }
        else {
          ANum2 = false;
        }
      }
      if (userInput[i + 5] == '/' && userInput[i + 6] == '>') { // retrieves 3rd input number
        if (userInput[i + 7] == '@') {
          if (userInput[i + 8] == '@') {
            if (userInput[i + 9] == '+') { // Value 1
              ANum3 = 1;
            }
            else if (userInput[i + 9] == '>') { // Value 2
              ANum3 = 2;
            }
            else if (userInput[i + 9] == '@') {
              ANum3 = 0;
            }
            else {
              ANum3 = false;
            }
          }
          else if (userInput[i + 8] == '+') {
            if (userInput[i + 9] == '@') {
              ANum3 = 3;
            }
            else if (userInput[i + 9] == '+') {
              ANum3 = 4;
            }
            else if (userInput[i + 9] == '>') {
              ANum3 = 5;
            }
            else {
              ANum3 = false;
            }
          }
          else if (userInput[i + 8] == '>') {
            if (userInput[i + 9] == '@') {
              ANum3 = 6;
            }
            else if (userInput[i + 9] == '+') {
              ANum3 = 7;
            }
            else if (userInput[i + 9] == '>') {
              ANum3 = 8;
            }
            else {
              ANum3 = false;
            }
          }
        }
      }
      else if (userInput[i + 7] == '+') {
        if (userInput[i + 8] == '@' && userInput[i + 9] == '@') {
          ANum3 = 9;
        }
        else {
          ANum3 = false;
        }
      }

      if (userInput[i + 10] == '/' && userInput[i + 11] == '>' && userInput[i + 12] == '@' && userInput[i + 13] == '@') { // retrieves 1st number again
        if (userInput[i + 14] == '@') {
          ANum1 = 0;
        }
        else if (userInput[i + 14] == '+') {
          ANum1 = 1;
        }
        else if (userInput[i + 14] == '>') {
          ANum1 = 2;
        }
      }
    }
    strSum += String(ANum2) + String(ANum3) + String(ANum1); // each number string concatenated
    finNum = strSum;
    i += 15; // index incremented by sacode length (15)
  }

  String commandString = String(num) + finNum;
  int lastIndex = commandString.length() - 1; // removes last value due to adding extra value at end in loop
  commandString.remove(lastIndex); // REF https://arduino.stackexchange.com/questions/20174/arduino-subtracting-chars-from-strings#:~:text=You%20may%20use%20remove()%20method%20to%20remove%20last%20character.

  String firstRVal = commandString;
  int firstValues = firstRVal.length() - 9; // removes last 9 numeric digits to retrieve first 3 digits
  firstRVal.remove(firstValues);
  long greenValues = firstRVal.toInt(); // converts string to int
  if (greenValues > 255) { // any value entered above max is set to max
    greenValues = 255;
  }
  ledPin = 10; // GREEN pin
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, greenValues);

  String secondRVal = commandString;
  int secondValues = secondRVal.length() - 6; // removes last 6 digits
  secondRVal.remove(secondValues);
  secondRVal.remove(0, 3); // removes first 3 digits
  long yellowValues = secondRVal.toInt();
  if (yellowValues > 255) {
    yellowValues = 255;
  }
  ledPin = 9; // YELLOW pin
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, yellowValues);

  String thirdRVal = commandString;
  int thirdValues = thirdRVal.length() - 3; // removes last 3 digits
  thirdRVal.remove(thirdValues);
  thirdRVal.remove(0, 6); // removes first 6 digits
  long orangeValues = thirdRVal.toInt();
  if (orangeValues > 255) {
    orangeValues = 255;
  }
  ledPin = 6; // ORANGE pin
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, orangeValues);

  String fourthRVal = commandString;
  fourthRVal.remove(0, 9); // removes first 9 digits so that only the last 3 digits remain
  long redValues = fourthRVal.toInt();
  if (redValues > 255) {
    redValues = 255;
  }
  ledPin = 5; // RED pin
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, redValues);
}

void VFCommand(String userInput) { // VFCommand takes userInput as parameter
  int counter;
  int VFNum;
  String VFTotal;
  int i = 7;
  for (i; i < 26; counter++) {
    if (userInput[i] == '/' && userInput[i + 1] == '>') { //
      if (userInput[i + 2] == '@') {
        if (userInput[i + 3] == '@') {
          if (userInput [i + 4] == '+') {
            VFNum = 1;
          }
          else if (userInput[i + 4] == '>') {
            VFNum = 2;
          }
          else if (userInput[i + 4] == '@') {
            VFNum = 0;
          }
          else {
            VFNum = false;
          }
        }
        else if (userInput[i + 3] == '+') {
          if (userInput [i + 4] == '@') {
            VFNum = 3;
          }
          else if (userInput[i + 4] == '+') {
            VFNum = 4;
          }
          else if (userInput[i + 4] == '>') {
            VFNum = 5;
          }
          else {
            VFNum = false;
          }
        }
        else if (userInput[i + 3] == '>') {
          if (userInput[i + 4] == '@') {
            VFNum = 6;
          }
          else if (userInput[i + 4] == '+') {
            VFNum = 7;
          }
          else if (userInput[i + 4] == '>')  {
            VFNum = 8;
          }
          else {
            VFNum = false;
          }
        }
      }
      else if (userInput[i + 2] == '+') {
        VFNum = 9;
      }
      else {
        VFNum = false;
      }
    }
    i += 5;
    VFTotal += String(VFNum); // adds value to total before looping again
  }
  int totalInt = VFTotal.toInt();
  VFLight(totalInt);
}

#define del 125

void VFLight(int total) {
  
  byte ledPins[] = {5, 9, 6, 10}; // led pin array (red, yellow, orange, green)
  
  int VFNum1;
  int VFNum2;
  int VFNum3;
  int VFNum4;

  VFNum1 = total / 1000; // finds first digit of total int
  VFNum2 = (total / 100) % 10; // finds second digit of total int
  VFNum3 = (total / 10) % 10; // finds third digit of total int
  VFNum4 = total % 10; // finds fourth digit of total int

  pinMode(ledPins[0], OUTPUT); // sets all ledpins to output
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  for (int i = 0; i < 10; i++) {
    if (VFNum1 == 0) { // if a number is equal to zero then the pin is removed so that the led does not flash
      ledPins[0] = 0;
    }
    if (VFNum2 == 0) {
      ledPins[1] = 0;
    }
    if (VFNum3 == 0) {
      ledPins[2] = 0;
    }
    if (VFNum4 == 0) {
      ledPins[3] = 0;
    }
    if (VFNum1 >= 1 || VFNum2 >= 1 || VFNum3 >= 1 || VFNum4 >= 1) { // at least one value must be greater than or equal to 1 to output a digit
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[1], HIGH);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[3], HIGH);
      delay(125);
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[2], LOW);
      digitalWrite(ledPins[3], LOW);
      delay(125);
    }
    VFNum1 -= 1; // reduction of each number by 1 until the value is equal to 0
    VFNum2 -= 1;
    VFNum3 -= 1;
    VFNum4 -= 1;
  }
}
