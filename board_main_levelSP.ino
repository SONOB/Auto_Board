#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define SPMAX 90 //모터 최대출력값 지정 BLDC모터이기 때문에 75도 상당히 위험함 따라서 낮춰쓰는것을 추천
Servo motor1,motor2;

int val1 = 60; //왼쪽바퀴 기본 모터값 설정
int val2 = 60; //오른쪽바퀴 기본 모터값 설정
RF24 radio(7, 8);
const byte address[6] = "02019"; //송신기와 동일하게 설정 컨트롤러에도 02019로 설정되 있을 것임
struct Data{
  byte joyX;
  byte joyY;
  byte joyZ;
  };
Data data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(true);
  motor1.attach (5);
  motor2.attach (6);
  motor1.write(60);
  motor2.write(60);
  delay(3000);
  
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX); //수신과 동일하게 MAX로 설정(MIN, LOW, HIGH, MAX)
  radio.startListening(); //데이터 수신
}

void loop() {
  while (radio.available()) {
    radio.read(&data, sizeof(Data));
    if (val1 > SPMAX || val2 > SPMAX){ //속도가 지정한 최대값이 넘으면 다시 60으로 고정
      val1 = 60;
      val2 = 60;
    }
    else{
      if(data.joyX < 10){
        val1 = 70;
        val2 = 70;
      }
      if(data.joyX > 169){
        val1 = 65;
        val2 = 65;
      }
      if(data.joyY > 169){ 
        val1 = 75;
        val2 = 75;
      }
      if(data.joyY < 10){ 
        val1 = 80;
        val2 = 80;
      }
      if(data.joyZ == 0){
        val1 = 60;
        val2 = 60;
      }
      motor1.write(val1);
      motor2.write(val2);
    }
  }
    Serial.println(data.joyX);
    Serial.println(data.joyY);
    Serial.print("value 1 :");
    Serial.println(val1);
    Serial.print("value 2 :");
    Serial.println(val2);
    delay(10); 

}
