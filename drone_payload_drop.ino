/*
   Drone Payload Drop

   TO DO:
   - Test relay & voltage output
   - Setup Arduino for power on drone PDB
   - Install hardware on drone
*/

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

  delay(10000);

  Serial.print("Load munitions...");
  munitionsSetup();
  Serial.println("secured.");

  digitalWrite(ledPin, LOW);
}

void loop() {
  int buzzPwrVal, buzzGndVal;
  int buzzVal, buzzLoopAvg;
  int loopTotal = 0;
  int loopCount = 0;

  for (unsigned long startTime = millis(); (millis() - startTime) < 2000; ) {
    buzzPwrVal = analogRead(buzzPwr);
    buzzGndVal = analogRead(buzzGnd);
    buzzVal = buzzPwrVal - buzzGndVal;

    loopTotal += buzzVal;
    loopCount++;

    delay(50);
  }
  buzzLoopAvg = loopTotal / loopCount;

  Serial.print("FC Input: ");
  Serial.println(buzzLoopAvg);

  if (buzzLoopAvg > 500) {
    digitalWrite(ledPin, HIGH);
    munitionsDrop();
  }
}

void munitionsSetup() {
  clawServo.attach(servoPin);
  clawServo.write(140);
  delay(5000);
  clawServo.write(170);
  delay(2000);
  clawServo.detach();
}

void munitionsDrop() {
  Serial.println("Dropping munitions!");
  digitalWrite(relayPin, HIGH);
  // delay(4000);  // Allow time for fuse ignition
  clawServo.attach(servoPin);
  clawServo.write(120); // Close to maximum opened travel range for claw
  delay(2000);
  clawServo.detach();

  while (true) {
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
  }
}
