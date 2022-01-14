#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

char choice;
SoftwareSerial bluetoothDevice(0, 1);

Servo servo;
LiquidCrystal_I2C lcd(0x27,16,2);

const byte ROWS = 4; //four rows!
const byte COLS = 3; //three columns
const int buzzer = A1; //buzzer to arduino pin A1

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char key_code[3];

unsigned int k=0;
unsigned int i=0;

byte rowPins[ROWS] = {8, 7, 6, 5}; 
byte colPins[COLS] = {4, 3, 2}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

void setup(){
  Serial.begin(9600);
  bluetoothDevice.begin(9600);
  pinMode(buzzer, OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); 
  servo.attach(A0);
  servo.write(50);
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  Welcome To ");
  lcd.setCursor(0,1);
  lcd.print("   Our Home  "); 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Enter the pass ");
  lcd.setCursor(0,1);
  lcd.print(" Scan the card  "); 
}

void loop(){
  char key = keypad.getKey();
  
    if(key != NO_KEY){
    Serial.println(key);
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print(key);
    Serial.println("pressed:");    
    Serial.print(i);    
     key_code[i++]=key;
     k=i;
    delay(200);
     }
     
 if(k==3){
   if(key_code[0]=='2’&&key_code[1]=='4’&&key_code[2]=='5')
   {
   lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Access Granted"); delay(500); lcd.clear();
   tone(buzzer, 1000); // Send 1KHz sound signal...
   delay(1000);        // ...for 1 sec
   noTone(buzzer);
   servo.write(140);
   delay(500);
   delay(3000);   
   servo.write(50);
   lcd.setCursor(0, 0);
   lcd.print("Enter the Pass");
   lcd.setCursor(0,1);
   lcd.print(" Scan the card  ");}
   if(key_code[0]!='2' || key_code[1]!='4' || key_code[2]!='5'){
   lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Access Denial"); delay(500); lcd.clear();
   i=k=0; 
   lcd.setCursor(0, 0);
   lcd.print("Enter the Pass");
   lcd.setCursor(0,1);
   lcd.print(" Scan the card  ");}
   i=k=0;    
 }

if(bluetoothDevice.available())
  {
    choice = bluetoothDevice.read();
    if (choice == '1')
    {
      lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Door Unlocked");
      tone(buzzer, 1000); // Send 1KHz sound signal...     // ...for 1 sec
      noTone(buzzer);
      servo.write(140);
    }
    else if(choice == '0')
    {
      lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Door Locked");
      servo.write(50);
    }
  }


if (! rfid.PICC_IsNewCardPresent()){
  return;
 }
if (! rfid.PICC_ReadCardSerial()){
  return;
 }

String content = "";
byte letter;
for (byte i = 0; i < rfid.uid.size; i++){
  content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
  content.concat(String(rfid.uid.uidByte[i], HEX));
}

content.toUpperCase();
if (content.substring(1) == "A9 ED F0 4B" || content.substring(1) == "41 4F D8 3F")
{
   lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Access Granted"); delay(500); lcd.clear();
   tone(buzzer, 1000); // Send 1KHz sound signal...
   delay(1000);        // ...for 1 sec
   noTone(buzzer);
   servo.write(140);
   delay(500);
   delay(3000);   
   servo.write(50);
   lcd.setCursor(0, 0);
   lcd.print("Enter the Pass");
   lcd.setCursor(0,1);
   lcd.print(" Scan the card  ");}
}