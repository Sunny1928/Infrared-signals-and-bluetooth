#include <IRLibRecvPCI.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
IRrecvPCI myReceiver(2);  // Arduino UNO pin 2
IRsendRaw mySender;

#define RAW_DATA_LEN 200
// uint16_t rawDataON[RAW_DATA_LEN]={
// 8950, 4614, 506, 1802, 506, 1802, 506, 690, 
// 506, 690, 506, 690, 506, 690, 506, 690, 
// 506, 1802, 506, 686, 506, 1802, 506, 1802, 
// 506, 690, 506, 690, 506, 690, 506, 690, 
// 506, 690, 506, 1798, 506, 1802, 506, 1802, 
// 506, 690, 506, 690, 506, 690, 506, 690, 
// 506, 690, 506, 690, 506, 1798, 510, 690, 
// 506, 690, 506, 690, 506, 1802, 506, 690, 
// 506, 1802, 506, 690, 506, 690, 506, 690, 
// 506, 690, 506, 690, 506, 690, 506, 694, 
// 502, 694, 506, 690, 506, 690, 506, 690, 
// 506, 690, 506, 694, 506, 690, 506, 690, 
// 506, 670, 466, 1000, 0, 0, 0, 0, 0, 0, 0};

uint16_t rawDataON[RAW_DATA_LEN]={};

void setup() {
  Serial.begin(9600);
  myReceiver.enableIRIn();
  Serial.println("Start");
}

void loop() {
  if(Serial.available()){
    char c = Serial.read();          //讀入手機App藉由藍牙輸入 Arduino 的字元，字元存入c
    Serial.println(c);           
     
    switch(c){
      case '1':                     //當讀入字元為 1，打開家電
        send_raw_data();
        break;
        
      case '0':                     //當讀入字元為0，關掉家電
        receive_raw_data();
        break;
        
      default:
        break;
    }
  }
  delay(100);
}

// 傳送紅外線編碼
void send_raw_data(){
  delay(5000);
  Serial.println("Sending ON");
  mySender.send(rawDataON, RAW_DATA_LEN, 38);
  Serial.println("Sent Turn ON Aircon");
  digitalWrite(13,!digitalRead(13));
  // printAll();
}

// 接收紅外線編碼
void receive_raw_data(){
  myReceiver.disableIRIn();
  myReceiver.enableIRIn();
  delay(3000);
  if (myReceiver.getResults()) {
    Serial.println("raw data");
    for (bufIndex_t i = 0; i < recvGlobal.recvLength; i++) {
      rawDataON[i] = recvGlobal.recvBuffer[i+1];
    }
    // printAll();
    myReceiver.enableIRIn();
  }
  
}

// 印出現在存的所有編碼
void printAll(){
  Serial.println("ALL");
  for (bufIndex_t i = 0; i < RAW_DATA_LEN; i++) {
    Serial.print(rawDataON[i], DEC);
    Serial.print(F(", "));
    if ((i % 8) == 0) {
      Serial.print(F("\n"));
    }
  }
}
