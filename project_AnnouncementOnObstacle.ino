// Libraries:
#include <SoftwareSerial.h>
/* Built-In Library */

#include <DFRobotDFPlayerMini.h>
/* Install: "DFRobotDFPlayerMini Library by DFRobot" */

// Pin Numbers: Ultrasonic Sensor
#define US_Trig_Pin 4   // (OUTPUT) Connect to Trig pin
#define US_Echo_Pin 5   // (INPUT)  Connect to Echo pin

// Pin Numbers: DF Player
#define DFP_Rx_Pin 6  //Connect to Tx of DF Player Module
#define DFP_Tx_Pin 7  //Connect to Rx of DF Player Module

// Configurations:
#define Volume 30         // Set volume value. From 0 to 30
#define DistThreshold 70  // Distance Threshold
#define TrackNum 1

// Software Serial Port:
SoftwareSerial SerialDFP(DFP_Rx_Pin, DFP_Tx_Pin);

// Objects:
DFRobotDFPlayerMini objDFP;

// Variables:
int distance;

void setup() {
  // Define Input & Output Pins:
  pinMode(US_Trig_Pin, OUTPUT);
  pinMode(US_Echo_Pin, INPUT);

  // Initialize serial communications with the PC:
  Serial.begin(9600);

  // Initialize DFPlayer:
  SerialDFP.begin(9600);
  Serial.println();
  Serial.println("DFRobot DFPlayer Mini");
  Serial.println("Initializing DFPlayer module ... Wait!");

  if (!objDFP.begin(SerialDFP)) {
    Serial.println("DFPlayer is not initialized:");
    Serial.println("1. Check the DFPlayer Mini connections");
    Serial.println("2. Insert an SD card");
    while (true);
  }
  Serial.println();
  Serial.println("DFPlayer Mini module initialized!");
  objDFP.volume(Volume);      
}

void loop() {
  if (GetUltrasonicAvgDist(US_Trig_Pin, US_Echo_Pin, 3) < DistThreshold) {
    objDFP.play(TrackNum);
    delay(5000);
  }
}

int GetUltrasonicAvgDist(int trigPin, int echoPin, int n) {
  int distSum = 0;
  for (int i = 1; i <= n; i++) {
    /* Clear the trig pin by setting it LOW */
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);

    /* Trigger the sensor by setting the trig pin HIGH for 10 microseconds */
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    /* Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds */
    long duration = pulseIn(echoPin, HIGH);

    /* Calculate the distance: Distance = Speed * Time
       Speed of sound is 343 metres per second = 0.0343cm per micro second */
    distSum += 0.0343 * (duration / 2);

    delay(50);
  }
  int distance = distSum / n;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}
