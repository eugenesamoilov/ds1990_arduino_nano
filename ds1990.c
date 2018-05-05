

#include <OneWire.h>
#define pin 10
#define button_pin 3
OneWire ibutton (pin); // I button connected on PIN 2.

byte addr[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //array to store the Ibutton ID.

boolean buttonWasUp = true;//была ли кнопка отпущена
boolean flage = false;

void setup(){
 Serial.begin(115200); 
 pinMode(button_pin, INPUT_PULLUP);
// pinMode (13, OUTPUT);
}

void loop(){

if (flage==false)  if (!ibutton.search (addr)){//read attached ibutton and asign value to buffer
    ibutton.reset_search();
    delay(200);
if (addr[0]==0xFF,addr[1]==0xFF,addr[2]==0xFF,addr[3]==0xFF,addr[4]==0xFF,addr[5]==0xFF,addr[6]==0xFF,addr[7]==0xFF) flage=false;
else flage=true;
    return;
 }
//   if (flage==true){
//      digitalWrite(13,HIGH);    
//   }else digitalWrite(13,LOW);
 
  Serial.print(millis()/1000);
  Serial.print("> ");
  for (byte x = 0; x<8; x++){  
     Serial.print(addr[x],HEX); //print the buffer content in LSB. For MSB: for (int x = 8; x>0; x--) 
     Serial.print(" ");
  }
 
  //compute crc//
  byte crc;
  crc = ibutton.crc8(addr, 7);
  Serial.print("CRC: ");
  Serial.println(crc,HEX);
 
  Serial.println(flage);

  boolean buttonIsUp = digitalRead(button_pin);

  if (buttonWasUp && !buttonIsUp){
    delay(10);
    buttonIsUp = digitalRead(button_pin);
    if(!buttonIsUp){
      // это клик
        
    ibutton.skip();ibutton.reset();ibutton.write(0x33);
    Serial.print("  ID before write:");
    for (byte x=0; x<8; x++){
      Serial.print(' ');
      Serial.print(ibutton.read(), HEX);
    }
    // send reset
    ibutton.skip();
    ibutton.reset();
    // send 0xD1
    ibutton.write(0xD1);
    // send logical 0
    digitalWrite(pin, LOW); pinMode(pin, OUTPUT); delayMicroseconds(60);
    pinMode(pin, INPUT); digitalWrite(pin, HIGH); delay(10);
    
    Serial.print('\n');
    Serial.print("  Writing iButton ID:\n    ");
    // Hardcode here your desired ID //
    // 01 D5 9F DC 02 00 00 96
//    byte newID[8] =   { 0x01, 0x5A, 0x75, 0x95, 0x6, 0x0, 0x0, 0xD2 };
                     
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);
    for (byte x = 0; x<8; x++){
      writeByte(addr[x]);
      Serial.print('*');
    }
    Serial.print('\n');
    ibutton.reset();
    // send 0xD1
    ibutton.write(0xD1);
    //send logical 1
    digitalWrite(pin, LOW); pinMode(pin, OUTPUT); delayMicroseconds(10);
    pinMode(pin, INPUT); digitalWrite(pin, HIGH); delay(10);
    flage = false;
    digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  buttonWasUp = buttonIsUp;
} 

int writeByte(byte data){
  int data_bit;
  for(data_bit=0; data_bit<8; data_bit++){
    if (data & 1){
      digitalWrite(pin, LOW); pinMode(pin, OUTPUT);
      delayMicroseconds(60);
      pinMode(pin, INPUT); digitalWrite(pin, HIGH);
      delay(10);
    } else {
      digitalWrite(pin, LOW); pinMode(pin, OUTPUT);
      pinMode(pin, INPUT); digitalWrite(pin, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}
