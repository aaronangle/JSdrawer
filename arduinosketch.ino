#include <Servo.h>
Servo myservo;

#define xDirPin 5
#define xStepPin 2
#define yDirPin 6
#define yStepPin 3
//#define zPin 11
#define enablePin 8

//LOW - counter clockwise HIGH - clockwise
//Pin 11 is connected to the z+- for the servo
//2 is x step
//3 is y step
//5 is x dir
//6 is y dir

String incomingCoords = "";
int x = 0;
int y = 0;
bool penDown = false;
int servoPos = 90;
int servoMove = 35;

void putDownPen() {
  int move = 0;
  penDown = true;
  while (move < servoMove) {
    move++;
    myservo.write(servoPos + move);
    delay(10);
  }
  servoPos = servoPos + move;
  Serial.println(servoPos);
}

void pickUpPen() {
  int move = 0;
  penDown = false;
  while (move < servoMove) {
    move++;
    myservo.write(servoPos - move);
    delay(10);
  }

  servoPos = servoPos - move;
  Serial.println(servoPos);
}

void setup()
{
  Serial.begin(9600);

  // Declare pins as output:
  pinMode(xStepPin, OUTPUT);
  pinMode(xDirPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);  // ******** set the enable pinMode
  digitalWrite(enablePin, LOW); // ******** enable the motors
  myservo.attach(11);
  delay(100);
  putDownPen();
}


void movePen(String axis, int coord) {
  int steps = 0;
  bool clockWise = true;

  steps = coord - (axis == "x" ? x : y);

  clockWise = steps < 0;

  if ((axis == "x" && x == 0) || (axis == "y" && y == 0)) {
    clockWise = false;
  }

  int stepCount = abs(steps);

  digitalWrite(axis == "x" ? xDirPin : yDirPin, clockWise ? HIGH : LOW);

  int delayTime = 0;

  if (stepCount == 1) {
    delayTime = 20;
  } else if (stepCount < 11) {
    delayTime = 700;
  } else {
    delayTime = 3000;
  }

  for (int i = 0; i < stepCount; i++)
  {
    digitalWrite(axis == "x" ? xStepPin : yStepPin, HIGH);
    digitalWrite(axis == "x" ? xStepPin : yStepPin, LOW);
    delayMicroseconds(delayTime);
  }

  if (axis == "x") {
    x = coord;
  } else {
    y = coord;
  }
}

void loop() {
  incomingCoords = Serial.readStringUntil('\n');
  if (incomingCoords.length() > 0 && incomingCoords != "-1") {
    int commaIndex = incomingCoords.indexOf(',');

    int xCoord = incomingCoords.substring(0, commaIndex).toInt();
    int yCoord = incomingCoords.substring(commaIndex + 1, incomingCoords.length()).toInt();

    if (abs(xCoord - x) > 1 || abs(yCoord - y) > 1) {
      pickUpPen();
    }

    movePen("x", xCoord);
    movePen("y", yCoord);

    if (!penDown) {
      if (x == 0 && y == 0) {
        delay(5000);
      }
      if (xCoord != 0 || yCoord != 0) {
        putDownPen();
      }
    }

    Serial.println("done");
  }
}

void testPen() {
  pickUpPen();
  delay(1000);
  putDownPen();
  delay(1000);

}
