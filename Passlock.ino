#include <Keypad.h>
#include <EEPROM.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8,9}; //connect to the column pinouts of the keypad

byte buzzer = 10;
byte lock = 11; // Relay Pin
bool islock = true;
byte state = 0;
String password = "";
String input = "";

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void pass_save()
{
  
  
  EEPROM.write(0, password.length());
  for(int i = 0; i<password.length(); i++)
  {
    EEPROM.write(i+1, password.charAt(i));
  }
}
void pass_collect()
{
  byte len = EEPROM.read(0);
  password = "";
  for(int i = 0; i<len ; i++)
  {
    char c = EEPROM.read(i+1);
    password.concat(c);
  }
}
void passcheck()
{
  if(input.length() < password.length() && islock)
  {
    char customKey = customKeypad.getKey();
  
    if (customKey){
      input.concat(customKey);
      Serial.print(customKey);
      tone(buzzer,2000,50);
      
    }
  }
  else if(!islock)
  {
    char customKey = customKeypad.getKey();
    
      if(customKey != NULL)
      {
        digitalWrite(lock,HIGH);
        islock =  true;
        tone(10,1500,400);
        delay(450);
        tone(10,5500,100);
        delay(150);
        tone(10,5500,100);
      }
    
    
  }
  
  else if(input.length() == password.length())
  {
    Serial.println("");
    
    if(input.equals(password))
    {
      
      Serial.println("Password matched!");
      digitalWrite(lock,LOW);
      tone(10,1500,400);
      delay(450);
      tone(10,5500,100);
      delay(150);
      tone(10,5500,100);
      islock = false;
      
    }
    else
    {
      Serial.println("Wrong password!");
      Serial.print(","+input+",");
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      
      
    }
    input="";
  }
}
int passvalue()
{
  int val = 0;
  if(input.length() < password.length())
  {
    char customKey = customKeypad.getKey();
  
    if (customKey){
      input.concat(customKey);
      Serial.print(customKey);
      tone(buzzer,2000,50);
      
    }
  }
  
  
  else if(input.length() == password.length())
  {
    Serial.println("");
    if(input.equals(password))
    {
      tone(10,1500,400);
      delay(450);
      tone(10,5500,100);
      delay(150);
      tone(10,5500,100);
      val = 1;
      
    }
    else
    {
      Serial.println("Wrong password!");
      Serial.print(","+input+",");
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      val = -1;
      
      
    }
    input="";
  }
  return val;
}

bool passchange()
{
  while(state == 2)
  {
    
      char customKey = customKeypad.getKey();
    
      if (customKey){
        input.concat(customKey);
        Serial.print(customKey);
        tone(buzzer,2000,50);
        
      }
    
  }
  Serial.println("Password accepted!");
  input.remove(input.length()-1);
  if(input.length()>=4)
  {
      password = input;
      
      
      pass_save();
      Serial.println("\nPassword Changed!!");
      tone(10,1500,400);
      delay(450);
      tone(10,5500,100);
      delay(150);
      tone(10,5500,100);
      tone(10,1500,400);
      delay(450);
      tone(10,5500,100);
      delay(150);
      tone(10,5500,100);
  }
  else
  {
    Serial.println("\nPassword is too short!");
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
      tone(buzzer,2500,50);
      delay(100);
    
  }
  input = "";
  
  
}


void setup(){
  
  pinMode(lock,OUTPUT);
  digitalWrite(lock,HIGH);
  Serial.begin(9600);
  pass_collect();
  
  customKeypad.addEventListener(keypadEvent);
  
  
}


  
void loop(){
  if(state == 0)
  {
    
    passcheck();
  }
  else if (state == 1)
  {
    tone(buzzer,2500,1000);
    input = "";
    Serial.println("");
    Serial.println("Old Password: ");
    
      while(1)
      {
        int val = passvalue();
        
        if(val == -1)
        {
          state = 0;
          break;
        }
        else if(val == 1)
        {
          Serial.println("value: " + val);
          state = 2;
          break;
        }
      }
      Serial.println("Old Password given");
      
    
    
  }
  else if(state == 2)
  {
    tone(buzzer,2500,1000);
    input = "";
    Serial.println("New Password: ");
    passchange();
      
    
  }
  
  
}
void keypadEvent(KeypadEvent key){
    switch (customKeypad.getState()){
    case HOLD:
        if (key == '*' && state == 0) {
            state  = 1;
        }
        else if (key == '#' && state == 2) {
            state  = 0;
        }
        break;
    
        
    }
    
}
