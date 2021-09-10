#include<TimerOne.h>

int signal1[] = {23, 25, 27};
int signal2[] = {46, 48, 50};
int signal3[] = {13, 12, 11};
int signal4[] = {10, 9, 8};

int t1signal = 52;
int t2signal = 53;

int redDelay = 2000;
int yellowDelay = 1000;
int traindelay = 5000;

volatile int triggerpin1 = 31;
volatile int echopin1 = 29;
volatile int triggerpin2 = 44;
volatile int echopin2 = 42;
volatile int triggerpin3 = 7;
volatile int echopin3 = 6;
volatile int triggerpin4 = 5;
volatile int echopin4 = 4;

volatile int t1TriggerPin = 37;
volatile int t1EchoPin = 36;
volatile int t2TriggerPin = 41;
volatile int t2EchoPin = 40;

volatile long time;                    // Variable for storing the time traveled
volatile int S1, S2, S3, S4, t1_s, t2_s;           // Variables for storing the distance covered

int t_road = 500;  // distance under which it will look for vehicles.
int t_train = 1000; // distance under for train deection

#include <LiquidCrystal.h>
LiquidCrystal lcd(32, 33, 34, 35, 38, 39);

const int buzzer = 43;
void setup() {
  Serial.begin(9600);
  Timer1.initialize(10000000);    //Begin using the timer.
  //This function must be called first.
  //"microseconds" is the period of time the timer takes.

  Timer1.attachInterrupt(softInterr); //Run a function each time the timer period finishes.

  // Declaring LED pins as output
  for (int i = 0; i < 3; i++) {
    pinMode(signal1[i], OUTPUT);
    pinMode(signal2[i], OUTPUT);
    pinMode(signal3[i], OUTPUT);
    pinMode(signal4[i], OUTPUT);
  }

  //for trains led
  pinMode(t1signal, OUTPUT);
  pinMode(t2signal, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Declaring ultrasonic sensor pins as output
  pinMode(triggerpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(triggerpin2, OUTPUT);
  pinMode(echopin2, INPUT);
  pinMode(triggerpin3, OUTPUT);
  pinMode(echopin3, INPUT);
  pinMode(triggerpin4, OUTPUT);
  pinMode(echopin4, INPUT);

  //for trains detection
  pinMode(t1TriggerPin, OUTPUT);
  pinMode(t1EchoPin, INPUT);

  pinMode(t2TriggerPin, OUTPUT);
  pinMode(t2EchoPin, INPUT);

  lcd.begin(16, 2);
  lcd.print("lcd");
  lcd.setCursor(0, 1);
  delay(1000);
  lcd.clear();
}

void loop()
{

  // if there's train coming from one direction
  if (t1_s < t_train)
  {
    train1Signal();
  }

  // if there's train coming from another direction
  else if (t2_s < t_train)
  {
    train2Signal();
  }
  else {
    // If there are vehicles at signal 1
    if (S1 < t_road)
    {
      signal1Function();
    }

    // If there are vehicles at signal 2
    if (S2 < t_road)
    {
      signal2Function();
    }

    // If there are vehicles at signal 3
    if (S3 < t_road)
    {
      signal3Function();
    }

    // If there are vehicles at signal 4
    if (S4 < t_road)
    {
      signal4Function();
    }

  }
}

// This is interrupt function and it will run each time the timer period finishes. The timer period is set at 100 milli seconds.
void softInterr()
{
  // Reading from first trains' ultrasonic sensor
  digitalWrite(t1TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(t1TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(t1TriggerPin, LOW);
  time = pulseIn(t1EchoPin, HIGH);
  t1_s = time * 0.034 / 2;


  // Reading from second trains' ultrasonic sensor
  digitalWrite(t2TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(t2TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(t2TriggerPin, LOW);
  time = pulseIn(t2EchoPin, HIGH);
  t2_s = time * 0.034 / 2;



  // Reading from first ultrasonic sensor
  digitalWrite(triggerpin1, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerpin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin1, LOW);
  time = pulseIn(echopin1, HIGH);
  S1 = time * 0.034 / 2;

  // Reading from second ultrasonic sensor
  digitalWrite(triggerpin2, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerpin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin2, LOW);
  time = pulseIn(echopin2, HIGH);
  S2 = time * 0.034 / 2;

  // Reading from third ultrasonic sensor
  digitalWrite(triggerpin3, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerpin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin3, LOW);
  time = pulseIn(echopin3, HIGH);
  S3 = time * 0.034 / 2;

  // Reading from fourth ultrasonic sensor
  digitalWrite(triggerpin4, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerpin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin4, LOW);
  time = pulseIn(echopin4, HIGH);
  S4 = time * 0.034 / 2;


  // Print distance values on serial monitor for debugging
  Serial.print("S1: ");
  Serial.print(S1);
  Serial.print("  S2: ");
  Serial.print(S2);
  Serial.print("  S3: ");
  Serial.print(S3);
  Serial.print("  S4: ");
  Serial.println(S4);
  Serial.print("  T1: ");
  Serial.print(t1_s);
  Serial.print("  T2: ");
  Serial.println(t2_s);



}

void signal1Function()
{
  Serial.println("1");
  low();
  // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(signal1[0], LOW);
  digitalWrite(signal1[2], HIGH);
  lcd.clear();
  lcd.print("Signal 1 is moving");
  delay(500);
  delay(redDelay);

  // if there are vehicels at other signals

  if (t1_s < t_train)
  {
    train1Signal();
  }

  // if there's train coming from another direction
  else if (t2_s < t_train)
  {
    train2Signal();
  }

  else if (S2 < t_road || S3 < t_road || S4 < t_road)
  {
    // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    digitalWrite(signal1[2], LOW);
    digitalWrite(signal1[1], HIGH);
    delay(yellowDelay);
  }
}

void signal2Function()
{
  Serial.println("2");
  low();
  digitalWrite(signal2[0], LOW);
  digitalWrite(signal2[2], HIGH);
  lcd.clear();
  lcd.print("Signal 2 is moving");
  delay(500);
  delay(redDelay);


  if (t1_s < t_train)
  {
    train1Signal();
  }

  // if there's train coming from another direction
  else if (t2_s < t_train)
  {
    train2Signal();
  }
  else if (S1 < t_road || S3 < t_road || S4 < t_road)
  {
    digitalWrite(signal2[2], LOW);
    digitalWrite(signal2[1], HIGH);
    delay(yellowDelay);
  }
}

void signal3Function()
{
  Serial.println("3");
  low();
  digitalWrite(signal3[0], LOW);
  digitalWrite(signal3[2], HIGH);
  lcd.clear();
  lcd.print("Signal 3 is moving");
  delay(500);
  delay(redDelay);

  if (t1_s < t_train)
  {
    train1Signal();
  }

  // if there's train coming from another direction
  else if (t2_s < t_train)
  {
    train2Signal();
  }

  else if (S1 < t_road || S2 < t_road || S4 < t_road)
  {
    digitalWrite(signal3[2], LOW);
    digitalWrite(signal3[1], HIGH);
    delay(yellowDelay);
  }
}

void signal4Function()
{
  Serial.println("4");
  low();
  digitalWrite(signal4[0], LOW);
  digitalWrite(signal4[2], HIGH);
  lcd.clear();
  lcd.print("Signal 4 is moving");
  delay(500);
  delay(redDelay);

  if (t1_s < t_train)
  {
    train1Signal();
  }

  // if there's train coming from another direction
  else if (t2_s < t_train)
  {
    train2Signal();
  }

  else if (S1 < t_road || S2 < t_road || S3 < t_road)
  {
    digitalWrite(signal4[2], LOW);
    digitalWrite(signal4[1], HIGH);
    delay(yellowDelay);
  }
}

void train1Signal() // ??!! ???!!!
{
  Serial.println("Train 1");
  lcd.clear();
  lcd.print("Train 1 is moving");
  tone(buzzer, 1000);
  delay(300);
  noTone(buzzer);
  delay(1000);
  low();
  digitalWrite(t1signal, HIGH);
  delay(traindelay);
  digitalWrite(t1signal, LOW);


}


void train2Signal()
{
  Serial.println("Train 2");
  lcd.clear();
  lcd.print("Train 2 is moving");

  tone(buzzer, 1000);
  delay(300);
  noTone(buzzer);
  delay(1000);
  low();
  digitalWrite(t2signal, HIGH);
  delay(traindelay);
  digitalWrite(t2signal, LOW);


}


// Function to make all LED's LOW except RED one's.
// first disabling everything then enabling only red ones
void low()
{
  for (int i = 1; i < 3; i++)
  {
    digitalWrite(signal1[i], LOW);
    digitalWrite(signal2[i], LOW);
    digitalWrite(signal3[i], LOW);
    digitalWrite(signal4[i], LOW);
  }
  for (int i = 0; i < 1; i++)
  {
    digitalWrite(signal1[i], HIGH);
    digitalWrite(signal2[i], HIGH);
    digitalWrite(signal3[i], HIGH);
    digitalWrite(signal4[i], HIGH);
  }
}
