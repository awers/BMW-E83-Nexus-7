#include <Wire.h>
#include <LiquidCrystal_I2C.h>

unsigned long pauseTime = 0;
int flagACC = 0;
int IBUSreceiveByte = 0;
byte IBUSbyte[7] = {0,0,0,0,0,0,0}; 
int ButtonPressed = 0;
int KeyID = 0;
byte MINUS_DOWN[7] = {0, 0x50, 0x04, 0x68, 0x32, 0x10, 0x1E}; // - BUTTON PRESS
byte PLUS_DOWN[7] = {0, 0x50, 0x04, 0x68, 0x32, 0x11, 0x1F};  // + BUTTON PRESS
byte PREV_DOWN1[7] = {0, 0x50, 0x04, 0x68, 0x3B, 0x08, 0x0F};  // < BUTTON PRESS 1
byte PREV_DOWN2[7] = {0, 0x50, 0x04, 0xC8, 0x3B, 0x08, 0xAF};  // < BUTTON PRESS 2
byte NEXT_DOWN1[7] = {0, 0x50, 0x04, 0x68, 0x3B, 0x01, 0x06};  // > BUTTON PRESS 1
byte NEXT_DOWN2[7] = {0, 0x50, 0x04, 0xC8, 0x3B, 0x01, 0xA6};  // > BUTTON PRESS 2
byte VOICE_DOWN[7] = {0, 0x50, 0x04, 0xC8, 0x3B, 0x80, 0x27}; // VOICE BUTTON PRESS
byte RT_DOWN1[7] = {0, 0x50, 0x04, 0xFF, 0x3B, 0x00, 0x90};   // R/T BUTTON PRESS 1
byte RT_DOWN2[7] = {0, 0x50, 0x04, 0xFF, 0x3B, 0x40, 0xD0};   // R/T BUTTON PRESS 2

int incomingByte = 0;
LiquidCrystal_I2C lcd(0x27,20,4);   // Addr: 0x27, 20 chars & 4 lines

void setup() { 
  Serial1.begin(9600);
  lcd.init(); 
  lcd.backlight();
  Media.begin();
  System.begin();
} 
void loop() { 
  lcd.setCursor(0, 0);
  lcd.print("Nexus 7 BMW (mini)");
  if (Serial1.available() && ButtonPressed == 0) {  
    TryReadIBUSmsg();
  }
  if (ButtonPressed == 1) {
    switch (KeyID) {
      case 100:  // если клавиша не  опознана или кокоието ошибки в шине при чтении
        PrintKey("UNKNOWN key press");
        PrintCode(); 
      break;
      case 1: 
        Media.write(MEDIA_VOLUME_UP);
        PrintKey("VOL + key press");
        PrintCode(); 
      break;    
      case 2:
        Media.write(MEDIA_VOLUME_DOWN);
        PrintKey("VOL - key press");
        PrintCode(); 
      break;
      case 11:
        Media.write(MEDIA_NEXT);
        PrintKey("NEXT TRACK key press");
        PrintCode();  
      break;
      case 21:
        Media.write(MEDIA_PREVIOUS);
        PrintKey("PREV TRACK key press");
        PrintCode(); 
      break;
      case 31:
        Media.write(MEDIA_PLAY_PAUSE);
        PrintKey("VOICE key press");
        PrintCode(); 
      break;
      case 41:
        Media.write(MEDIA_BROWSER_BACK);
        PrintKey("R/T key press");
        PrintCode(); 
      break;
    } 
    ButtonPressed = 0;
    Serial1.flush();
  }
  delay(10);
}  
// чтение IBUS шины
void TryReadIBUSmsg() {
  IBUSreceiveByte = Serial1.read();
  if (IBUSreceiveByte==0x50) { 
    IBUSbyte[1]=0x50;   
    for (int i=2; i <= 6; i++) {   
      IBUSbyte[i] = Serial1.read();
      delay(10); 
    }
    KeyID=100;
    if(memcmp(IBUSbyte, PLUS_DOWN, 7) == 0)   { KeyID=1; } 
    if(memcmp(IBUSbyte, MINUS_DOWN, 7) == 0)  { KeyID=2; } 
    if(memcmp(IBUSbyte, NEXT_DOWN1, 7) == 0)  { KeyID=11; }
    if(memcmp(IBUSbyte, NEXT_DOWN2, 7) == 0)  { KeyID=11; } 
    if(memcmp(IBUSbyte, PREV_DOWN1, 7) == 0)  { KeyID=21; }
    if(memcmp(IBUSbyte, PREV_DOWN2, 7) == 0)  { KeyID=21; }  
    if(memcmp(IBUSbyte, VOICE_DOWN, 7) == 0 ) { KeyID=31; } 
    if(memcmp(IBUSbyte, RT_DOWN1, 7) == 0 )   { KeyID=41; }
    if(memcmp(IBUSbyte, RT_DOWN2, 7) == 0 )   { KeyID=41; }
    ButtonPressed=1; 
  } else { 
    ButtonPressed=0; 
  } 
  Serial1.flush(); 
} 
void PrintCode() {
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  for (int x=1; x < 7; x++) {
    lcd.print(IBUSbyte[x], HEX);
    lcd.print(" ");
  }
}
void PrintKey(String text) {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(text);
}