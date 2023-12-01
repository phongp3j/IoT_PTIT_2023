#define BLYNK_TEMPLATE_ID "TMPL6IyLipRyX"
#define BLYNK_TEMPLATE_NAME "thungracthongminh"
#define BLYNK_AUTH_TOKEN "WvGlxfASYqNnvyWxluQ31qSzEA1RKFSL"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Phon";
char pass[] = "Phong1234t";

BlynkTimer timer;

#define echoPin D5
#define trigPin D6
#include <Servo.h>
Servo servo;
long duration;
int distance; 
int binLevel = 0;
int servoPosition = 0;

// Declare function prototype
BLYNK_WRITE_DEFAULT();

void SMESensor()
{
  int ir = digitalRead(D7);
  if (ir == LOW)
  {    
    // Kiểm tra điều kiện trước khi quay servo
    if (distance > 3 && binLevel < 100)
    {
      servo.write(180);
      servoPosition = 180;
      for (int i = 0; i < 35; i++)
      {
        Blynk.virtualWrite(V2, servoPosition);
        ultrasonic(); 
        delay(100);
      }
      servo.write(0);
      servoPosition = 0;
      Blynk.virtualWrite(V2, servoPosition);
    }
  }
  if (ir == HIGH)
  {
    ultrasonic();
    delay(200);
  }
}

void ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  binLevel = (27-distance)/24*100;
  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, binLevel);

  // Kiểm tra nếu distance <= 3cm hoặc binLevel >= 100%, gửi thông báo đến Blynk
  if (distance <= 3 || binLevel >= 100)
  {
    Blynk.virtualWrite(V4, "RÁC ĐẦY, HÃY ĐỔ RÁC ĐỂ TIẾP TỤC\n SỬ DỤNG!");
  }
  else
  {
    Blynk.virtualWrite(V4, "Hãy cho tôi rác!");
  }
}

BLYNK_WRITE_DEFAULT()
{
  int value = param.asInt();
  if (value == 1 && distance > 3 && binLevel < 100)
  {
    servo.write(180);
    servoPosition = 180;
    Blynk.virtualWrite(V2, servoPosition);
  }
  else
  {
    servo.write(0);
    servoPosition = 0;
    Blynk.virtualWrite(V2, servoPosition);
  }
}

void setup()
{
  Serial.begin(9600);
  servo.attach(D2, 500, 2500);
  pinMode(D7, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Blynk.begin(auth, ssid, pass);
  delay(2000);
  timer.setInterval(1000L, SMESensor);
}

void loop() 
{
  Blynk.run();
  timer.run();
}
