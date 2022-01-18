//GABBAR - A Voice Assistant [B7-6CSE07]

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

#define SD_ChipSelectPin 53

TMRpcm sound;
LiquidCrystal_I2C lcd(0x27,16,2);

String voice;

void setup()
{
  Serial.begin(9600);

  sound.speakerPin=11;
               
  lcd.init(); //initialize 
  lcd.backlight();  //turns on backlight
  lcd.setCursor(5,0);
  lcd.print("GABBAR");
  lcd.setCursor(4,1);
  lcd.print("IS BACK!");
  
  for(int i=2;i<=8;i++)
  {
    pinMode(i,OUTPUT);
  }
 
  for(int i=2;i<=5;i++)
  {
    digitalWrite(i,HIGH);
  }
  digitalWrite(7,HIGH); //5V for thermistor sensor
  digitalWrite(8,HIGH); //5V for sd card module
  
    if (!SD.begin(SD_ChipSelectPin)) 
   {  
    Serial.println("SD fail");
    return;   // don't do anything more if not
   }
  else 
  {
    Serial.println("SD ok");
  }
   sound.play((char*)"intro.wav");

}

void loop() 
{
  while (Serial.available()){ //Check if there is an available byte to read
     delay(10); //Delay added to make thing stable 
    char c = Serial.read(); //Conduct a serial read
    if (c == '#') {break;} //Exit the loop when the # is detected after the word
    voice += c; //Shorthand for voice = voice + c
  }
  
  if (voice.length() > 0)
  ////////////////////////////////////////////////////////////////////
  {
    if(voice == "*Gabbar lights on"){           //Voice Command to ON Relay 01 
      digitalWrite(2, LOW);   //Relay 01 ON
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("LIGHTS ON");
      delay(1000);
      }
    
    else if(voice ==  "*Gabbar let's party") {     //Voice Command to ON Relay 03
      digitalWrite(3, LOW);   //Relay 02 ON
      sound.play((char*)"mehbooba.wav");
      sound.volume(5);
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("PLAYING MEHBOOBA");
      delay(1000);
      for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(250);
      }
      lcd.print("PLAYING MEHBOOBA");
    }

    else if(voice ==  "*Ham Kare to Kare kya bole to Bole kya") {     //Voice Command to ON Relay 03
      digitalWrite(3, LOW);   //Relay 02 ON
      sound.play((char*)"modi.wav");
      sound.volume(5);
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("WAH MODIJI WAH!!");
      /*lcd.setCursor(5,1);
      lcd.print("WAH!!!");*/
      delay(1000);
    }

    else if(voice =="*Gabbar show the temperature")
    {
      digitalWrite(4, LOW);   //Relay 03 ON
      lcd.clear();
      temperature();
    }
    
    else if(voice == "*Gabbar turn on fan") {     //Voice Command to ON Relay 04 
      digitalWrite(5, LOW); //Relay 04 ON
      lcd.clear(); 
      lcd.setCursor(1,0);
      lcd.print("FAN TURNED ON");
      delay(1000);//Relay 04 ON
    }
    
    
    ////////////////////////////////////////////////////////////////////
    
    else if(voice == "*Gabbar turn off light") {      //Voice Command to OFF Relay 01 
      digitalWrite(2, HIGH);   //Relay 01 OFF
      lcd.clear(); 
      lcd.setCursor(3,0);
      lcd.print("LIGHTS OFF");
      delay(1000);
      
    }
        
    else if(voice =="*Gabbar stop")
    {
      digitalWrite(3, HIGH);   //Relay 02 OFF
      sound.disable();
      lcd.clear();
      lcd.print("MUSIC STOPPED");
    }

    else if(voice == "*Gabbar clear temperature") {      //Voice Command to OFF Relay 02
      digitalWrite(4, HIGH);   //Relay 03 OFF
      lcd.clear(); 
      lcd.setCursor(4,0);
      lcd.print("CLEARED");
      delay(1000);
      
    }
    else if(voice == "*Gabbar turn off fan") {      //Voice Command to OFF Relay 02
      digitalWrite(5, HIGH);   //Relay 04 OFF
      lcd.clear(); 
      lcd.setCursor(1,0);
      lcd.print("FAN TURNED OFF");
    
    }
    
    ////////////////////////////////////////////////////////////////////
    
    else if(voice == "*Gabbar turn all devices on") {      //Voice Command to ON all Relays
      switchallon();                //All Relays ON
      lcd.clear();
      temperature();
      sound.play((char*)"mehbooba.wav");
      sound.volume(5);
      
    }
    else if(voice == "*Gabbar turn all devices off") {      //Voice Command to OFF all Relays
      switchalloff();               //All Relays OFF
      sound.disable();
      lcd.clear(); 
      lcd.setCursor(3,0);
      lcd.print("POWER DOWN");
      delay(1000);
    }
    
  voice=""; //Reset the variable after initiating
  }
}
    ////////////////////////////////////////////////////////////////////////

void switchalloff()               //Function for turning OFF all relays
{
  for(int i=2;i<=5;i++)
  {
    digitalWrite(i,HIGH);
  }
}

void switchallon()                //Function for turning ON all relays
{
 for(int i=2;i<=5;i++)
 {
  digitalWrite(i,LOW);
 }
}

void temperature()
{
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(A0);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  int f = (T * 9.0)/ 5.0 + 32.0; 

  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(6,0); //0th row, 6th column coz the previous5 is already occupied for "Temp:" 
  lcd.print(T);
  lcd.setCursor(11,0);
  lcd.print(" C");
}
