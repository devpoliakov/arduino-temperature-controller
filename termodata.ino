//library for LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//library for Card
#include <SPI.h>
#include <SD.h>
File myFile;
//char* inStringus = "";
int inStringus = 0;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);



// set sensor
const int  buttonPin3 = 5;
// set relay


int WorkMode = 1;

// set buttons
const int buttonPin2 = 7;
const int buttonPin1 = 6;
const int buttonPin0 = 5;

// set variables for 
int buttonSensor = 0;


int variableTemperaturePlus = 30;

// set variables for buttons
int buttonSelect = 0;
int buttonPlus = 0;
int buttonMinus = 0;

int modecount = 0;
int minutecount = 0;

String variableContainer = "";
int SDVariableContainer = 0;

char* modename[] = {"work: ","AMode.txt", "ATemp.txt", "BMode.txt", "BTemp.txt"};
int modevariable[] = {0, 0, 0, 0, 0};

int saverCount = 1;

void setup()
{
	// initialize the LCD
	lcd.begin();
        Serial.begin(9600);
        while (!Serial) {
          ; // wait for serial port to connect. Needed for Leonardo only
        }
  
        Serial.print("Initializing SD card...");
        pinMode(10, OUTPUT);

if (!SD.begin(10)) {
    Serial.println("initialization failed -- not so good!");
    return;
  }
  Serial.println("initialization done.");
  
   /////////////////////////////////
  /// Saver function
  /////////////////////////////////



  // re-open the file for reading:
  for (saverCount = 1; saverCount < 5; saverCount++){
    variableContainer = "";
  myFile = SD.open(modename[saverCount]);
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
     
      variableContainer = myFile.readStringUntil('\n');     
    }
    SDVariableContainer = variableContainer.toInt();
    modevariable[saverCount] = SDVariableContainer;
    Serial.print(modevariable[saverCount]);
    Serial.println('-');
    
    
    // set saved data to loop
   
  }   
  
  }
  //////////////////////////////
        
        pinMode(9, OUTPUT);
        pinMode(buttonPin2, INPUT);
        pinMode(buttonPin1, INPUT);
        pinMode(buttonPin0, INPUT);  
        
	// Turn on the blacklight and print a message.
	lcd.clear();
    lcd.backlight();
    lcd.print(modename[modecount]);
    
}



void loop()
{
  
  buttonSelect = digitalRead(buttonPin2);
  buttonPlus = digitalRead(buttonPin1);
  buttonMinus = digitalRead(buttonPin0);
  
  int sensorValue = analogRead(A0);
  int termocounter = 0;
  
  //digitalWrite(9, LOW);
  /**
  main loop
  */
  
  
  
  if((WorkMode == 0) ){
    Serial.println(WorkMode);
  }  else{
    // detect Time 
    // first mode
    if(modevariable[1] > 1){
    ///////////////////////
    /// visual part
    ///////////////////////
    lcd.clear();
    lcd.backlight();
    //lcd.print("temp: ");
    lcd.print(sensorValue);
    
    lcd.backlight();
    lcd.println("t1: ");
    lcd.print(modevariable[1]);
   
    
    ///////////////////////
    //sensor part 
    
    if (modevariable[2] < sensorValue){
    digitalWrite(9, HIGH);
    }
    else if ((modevariable[2] > sensorValue) and (sensorValue > (modevariable[2] - variableTemperaturePlus)) ){
      
      
    // disable relay 
    digitalWrite(9, LOW);
    // start counter
    minutecount++;
    if(minutecount == 60){
      minutecount = 0;
      /////////////////////
      // remove  function
        modevariable[1] = modevariable[1] - 1; 
          SD.remove(modename[1]);
          myFile = SD.open(modename[1] , FILE_WRITE);  
        if (myFile) {
          myFile.print(modevariable[1]);
          // close the file:
          myFile.close();
        }
      ///////////////////
      }
    
    } 
    
  }   

    
    
    // detect Time 
    // Second mode
    else if(modevariable[3] > 1){
    ///////////////////////
    /// visual part
    ///////////////////////
    lcd.clear();
    lcd.backlight();
    //lcd.print("temp: ");
    lcd.print(sensorValue);
    
    lcd.backlight();
    lcd.println("t1: ");
    lcd.print(modevariable[3]);
   
    
    ///////////////////////
    //sensor part 
    
    if (modevariable[4] < sensorValue){
    digitalWrite(9, HIGH);
    }
    else if ((modevariable[4] > sensorValue) and (sensorValue > (modevariable[4] - variableTemperaturePlus)) ){
      
      
    // disable relay 
    digitalWrite(9, LOW);
    // start counter
    minutecount++;
    if(minutecount == 60){
      minutecount = 0;
      /////////////////////
      // remove  function
        modevariable[3] = modevariable[3] - 1; 
          SD.remove(modename[3]);
          myFile = SD.open(modename[3] , FILE_WRITE);  
        if (myFile) {
          myFile.print(modevariable[3]);
          // close the file:
          myFile.close();
        }
      ///////////////////
      }
    
    } 
    
  }   
  else{
    digitalWrite(9, LOW);
    }
    
    ////////////////////
    /////////////////////
    //////////////////////
    delay(1000);
  }
  
  // end of main loop
  if (buttonSelect == HIGH) {
    modecount++;
    //reset mode name
      if(modecount == 5){
      modecount = 0;
      WorkMode = 1;
      } else {
      WorkMode = 0;
      
      }
      
      
      
    lcd.clear();
    lcd.backlight();
    lcd.print(modename[modecount]);
    lcd.backlight();
    lcd.print(modevariable[modecount]);
    delay(500);
  }
  
  ////////////////////
  // plus
  ///////////////////
    if (buttonPlus == HIGH) {
      
    modevariable[modecount] = modevariable[modecount] + 10;
    
    
    /////////////////////////  
    
    SD.remove(modename[modecount]);
    myFile = SD.open(modename[modecount] , FILE_WRITE);

  // if the file opened okay, write to it:
  
  if (myFile) {
    Serial.print(modevariable[modecount]);
    Serial.print("Writing to test.txt...");
    
    myFile.print(modevariable[modecount]);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    /////////////////////
    
    lcd.clear();
    lcd.backlight();
    lcd.print(modename[modecount]);
    lcd.backlight();
    lcd.print(modevariable[modecount]);
    
//////////////////////////////////

    delay(500);
  }
     
    ////////////////////
  // plus
  ///////////////////
    if (buttonMinus == HIGH) {
      
    modevariable[modecount] = modevariable[modecount] - 10;
    
    
    /////////////////////////  
    
    SD.remove(modename[modecount]);
    myFile = SD.open(modename[modecount] , FILE_WRITE);

  // if the file opened okay, write to it:
  
  if (myFile) {
    Serial.print(modevariable[modecount]);
    Serial.print("Writing to test.txt...");
    
    myFile.print(modevariable[modecount]);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    /////////////////////
    
    lcd.clear();
    lcd.backlight();
    lcd.print(modename[modecount]);
    lcd.backlight();
    lcd.print(modevariable[modecount]);
    
//////////////////////////////////

    delay(500);
  }
  else {
    lcd.backlight();
  }
	// Do nothing here...
}
