#include <Servo.h>

// 핀 정의
#define PIN_POTENTIOMETER A3   // 가변 저항 A3
#define PIN_SERVO         10   // 서보 디지털 핀 10
#define PIN_IR_SENSOR     A0    // 적외선 거리 센서 A0

#define LOOP_INTERVAL 50

Servo myservo;
unsigned long last_loop_time = 0;

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.write(90);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_pot, angle, a_ir;
  float distance;
  
  // 다음 이벤트 시간까지 대기
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // 가변 저항 값 읽기
  a_pot = analogRead(PIN_POTENTIOMETER);  // 0 ~ 1023

  // 서보 제어: 가변 저항 값을 서보 각도로 매핑 (0 ~ 180도)
  angle = map(a_pot, 0, 1023, 0, 180);
  myservo.write(angle);

  // 적외선 센서 값 읽기
  a_ir = analogRead(PIN_IR_SENSOR);  // 0 ~ 1023

  // 거리 계산: 선형 모델 사용
  // distance_raw = (6762.0 / (a_value - 9) - 4.0) * 10.0
  if (a_ir > 9) {  // 분모가 0이 되지 않도록 9보다 클 때만 계산
    distance = (6762.0 / (a_ir - 9) - 4.0) * 10.0;  // mm 단위
  }
  else {
    distance = -1;
  }

  // 0~6cm (60mm)
  if (distance > 60) {
  }
  else {
    distance = -1;  // 무시
  }

  // 시리얼 출력
  Serial.print("Pot ADC: ");
  Serial.print(a_pot);
  Serial.print(" => Angle: ");
  Serial.print(angle);
  
  Serial.print(" | IR ADC: ");
  Serial.print(a_ir);
  Serial.print(" => Distance: ");
  if (distance > 60)
    Serial.print(distance);
  else
    Serial.print("Error or <6cm");
  Serial.println(" mm");
}
