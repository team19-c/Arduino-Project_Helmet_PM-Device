#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX, TX

int Dir1Pin_A = 4;      // A 제어신호 1핀 (모터)
int Dir2Pin_A = 5;      // A 제어신호 2핀 (모터)
int SpeedPin_A = 10;    // A PWM제어를 위한 핀 (모터)
int Dir1Pin_B = 6;      // B 제어신호 1핀 (모터)
int Dir2Pin_B = 7;      // B 제어신호 2핀 (모터)
int SpeedPin_B = 11;    // B PWM제어를 위한 핀 (모터)

char command;           // 헬멧에서 불러오는 시리얼 데이터를 command로 설정
bool motorRunning = false;  // 처음에 모터 작동 상태를 !Running으로 설정

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(Dir1Pin_A, OUTPUT);             // A 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_A, OUTPUT);             // A 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_A, OUTPUT);            // A PWM제어핀 출력모드 설정
  pinMode(Dir1Pin_B, OUTPUT);             // B 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_B, OUTPUT);             // B 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_B, OUTPUT);            // B PWM제어핀 출력모드 설정
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    if (command == 'G' && !motorRunning) {   //헬멧 수신 데이터가 'Go'이고 현재 모터 작동 상태가 !Running이라면
      // 모터 최고 속도
      digitalWrite(Dir1Pin_A, HIGH);         //모터(A)가 시계 방향으로 회전
      digitalWrite(Dir2Pin_A, LOW);
      analogWrite(SpeedPin_A, 255);          //모터 속도를 최대로 설정
      digitalWrite(Dir1Pin_B, HIGH);         //모터(B)가 시계 방향으로 회전
      digitalWrite(Dir2Pin_B, LOW);
      analogWrite(SpeedPin_B, 255);          //모터 속도를 최대로 설정
      motorRunning = true;                   //모터 작동 상태를 Running으로 설정
    } else if (command == 'S' && motorRunning) { //헬멧 수신 데이터가 'Stop'이고 현재 모터 작동 상태가 Running이라면
      motorRunning = false;                  //모터 작동 상태를 !Running으로 설정
      // 모터 감속
      for (int i = 255; i >= 0; i -= 51) {
        analogWrite(SpeedPin_A, i);          //모터 속도를 점차 줄임
        analogWrite(SpeedPin_B, i);          //모터 속도를 점차 줄임
        delay(1000);
      }
      analogWrite(SpeedPin_A, 0);            // 모터 완전히 멈춤
      analogWrite(SpeedPin_B, 0);            // 모터 완전히 멈춤
    }
  }
}
