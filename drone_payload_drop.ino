#include <Servo.h>

Servo clawServo;

const int relayPin = 2;
const int servoPin = 9;
const int ledPin = 13;

const int buzzPwr = A0;
const int buzzGnd = A1;

void setup() {
  pinMode(relayPin, OUTPUT); digitalWrite(relayPin, LOW);
  pinMode(ledPin, OUTPUT); digitalWrite(ledPin, HIGH);

  Serial.begin(9600);

  clawServo.attach(servoPin);
  clawServo.write(90);
  delay(5000);
  clawServo.write(0);
  delay(5000);
  clawServo.detach();

  digitalWrite(ledPin, LOW);
}

void loop() {
  int buzzPwrVal, buzzGndVal;
  int buzzVal, buzzLoopAvg;
  int loopTotal = 0;
  int loopCount = 0;

  for (unsigned long startTime = millis(); (millis() - startTime) < 2000; ) {
    buzzPwrVal = analogRead(buzzPwr);
    delay(15);
    buzzGndVal = analogRead(buzzGnd);
    delay(15);
    buzzVal = buzzPwrVal - buzzGndVal;

    loopTotal += buzzVal;
    loopCount++;

    delay(20);
  }
  buzzLoopAvg = loopTotal / loopCount;

  if (buzzLoopAvg > 500) {
    digitalWrite(ledPin, HIGH);
    munitionsDrop();
  }
}

void munitionsDrop() {
  digitalWrite(relayPin, HIGH);
  delay(2500);
  clawServo.attach(servoPin);
  clawServo.write(180);
  delay(5000);
  clawServo.detach();

  while (true) {
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
  }
}

