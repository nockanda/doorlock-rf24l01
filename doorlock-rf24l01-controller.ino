#include <SPI.h>
#include "RF24.h"

#define btn 2
#define red_led 3
#define green_led 4

RF24 radio(9, 10); //CE, SS

uint8_t address[6] = "00001";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //PC와 아두이노간 통신라인
  pinMode(btn,INPUT_PULLUP);
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  radio.begin(); //아두이노-RF모듈간 통신라인
  radio.setPALevel(RF24_PA_LOW); 
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.stopListening(); //기본 : 송신모드
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(btn) == LOW){
    digitalWrite(red_led,HIGH);
    digitalWrite(green_led,LOW);
    sendmode();
    char text[] = "nockanda";
    radio.write(text,sizeof(text));
    Serial.print("Request = ");
    Serial.println(text);
    
    receivemode();

    bool is_timeout = false;
    unsigned long t = millis();
    while(true){
      if(radio.available()) break;
      if(millis() - t > 3000){
        is_timeout = true;
        break;
      }
    }
    if(is_timeout){
      Serial.println("슬레이브가 타임아웃 처리되었다!");
    }else{
      char text2[30];
      radio.read(text2,sizeof(text2));

      if(text2[0] == 'O' && text2[1] == 'K'){
        //응답완료!
        digitalWrite(red_led,LOW);
        digitalWrite(green_led,HIGH);
      }
  
      //Serial.print("Response = ");
      //Serial.println(text2);
    }
    delay(300);
    
  }
/*
  //송신모드
  sendmode();
  char text[] = "NOCKANDA!!";
  radio.write(text,sizeof(text));

  Serial.print("Request = ");
  Serial.println(text);


  //수신모드
  receivemode();

  bool is_timeout = false;
  unsigned long t = millis();
  while(true){
    if(radio.available()) break;
    if(millis() - t > 3000){
      is_timeout = true;
      break;
    }
  }

  if(is_timeout){
    Serial.println("슬레이브가 출타했다!");
  }else{
    char text2[30];
    radio.read(text2,sizeof(text2));

    Serial.print("Response = ");
    Serial.println(text2);
  }
  delay(1000);
  */
}

void sendmode(){
  radio.stopListening();
}
void receivemode(){
  radio.startListening();
}
