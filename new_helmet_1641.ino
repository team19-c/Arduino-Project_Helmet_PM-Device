#include <SoftwareSerial.h>

#define NUM_PRESSURE_SENSORS 5
#define BUZZER_PIN 5

int pressurePins[NUM_PRESSURE_SENSORS] = {A0, A1, A2, A3, A4};
int pressureValues[NUM_PRESSURE_SENSORS];

SoftwareSerial BTSerial(2, 3); // RX, TX

bool isPaired = false;
bool isBuzzed = false;
bool motorRunning = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  int detectedSensors = 0;
  for (int i = 0; i < NUM_PRESSURE_SENSORS; i++) {
    pressureValues[i] = analogRead(pressurePins[i]);
    if (pressureValues[i] > 100) { // 감지 임계값
      detectedSensors++;
    }
  }
  
  if (BTSerial.available()) {
    isPaired = true;
  }

  if (isPaired) { //페어링이 됐을 때
    if (detectedSensors >= 3) { //압력센서가 3개 이상 감지됐을 때
      if (!isBuzzed) { // '삐삐삐' 소리를 출력하지 않았을 때
        tone(BUZZER_PIN, 700, 300); // '삐삐삐' 소리
        delay(700); 
        isBuzzed = true;
      }
      if (!motorRunning) { //모터가 작동중이 아니라면
        BTSerial.write('G'); // Go signal
        motorRunning = true; //모터 작동시작 시그널
      }
    } else {
      isBuzzed = false; //압력센서가 3개 미만 감지됐을 때
      motorRunning = false; //모터 작동 중지 시그널
      int beepDelay = map(detectedSensors, 0, 2, 200, 1000);
      BTSerial.write('S'); // Slow down signal
      tone(BUZZER_PIN, 300);
      delay(beepDelay);
      noTone(BUZZER_PIN);
      delay(beepDelay);
    }
  } else {
    // 페어링이 끊겼는지 확인
    if (BTSerial.available() == false) {
      tone(BUZZER_PIN, 100); // 지속적인 '삐' 소리
    }
  }
}
