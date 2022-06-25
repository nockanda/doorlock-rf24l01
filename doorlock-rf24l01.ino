
#include <SPI.h>
#include "RF24.h"

#define relay 2

RF24 radio(9, 10); //CE, SS

uint8_t address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //PC와 아두이노간 통신라인
  pinMode(relay,OUTPUT);
  radio.begin(); //아두이노-RF모듈간 통신라인
  radio.setPALevel(RF24_PA_LOW); 
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.startListening(); //기본 : 수신모드
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()){
    //RF무선모듈쪽으로 뭔가 수신된값이 존재한다면~
    char text[30];
    radio.read(text,sizeof(text));

    Serial.println(text);

    String mytext = text;

    if(mytext == "nockanda"){
      Serial.println("매치");
      digitalWrite(relay,HIGH);
      delay(100);
      digitalWrite(relay,LOW);
    }else{
      Serial.println("노매치");
    }

    //마스터에게 응답할 준비
    //송신모드
    sendmode();
    char text2[] = "OK";
    radio.write(text2,sizeof(text2));

    //보낼거 다보내고..수신모드
    receivemode();
  }
}

void sendmode(){
  radio.stopListening();
}
void receivemode(){
  radio.startListening();
}