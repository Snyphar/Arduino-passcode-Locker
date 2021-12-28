#include <EEPROM.h>

String password = "123456";

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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pass_save();
  Serial.println("Password Saved!");
  pass_collect();
  Serial.println("your password is: " + password);
  
  
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
