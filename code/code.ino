#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

#include <Servo.h>
Servo irServo;
Servo pump_servo;

#define IRservo_pin 2 //orange
#define PumpServo_pin 12 // white
#define IR_1 A0 // right grey wire
#define IR_2 A1 // left purple

#define RIGHT_BACKWARD 8 //yellow
#define RIGHT_FORWARD 7 // green

#define LEFT_BACKWARD 5 //purple
#define LEFT_FORWARD 6 //blue

#define PWM 3 //motor speed control

#define trig 9 //brown
#define echo 10 // red

#define LED 11 // white

//9 10 11 12 13
int pos = 0;
int fireDetected = 0;
int servoDelay = 10;

int ir1 = 0;
int ir2 = 0;
int firereading = 400;

char irUsed;

boolean fire = false;
boolean objectFound = false;

long readUltrasonicDistance (int triggerPin, int echoPin)
{
  pinMode (triggerPin, OUTPUT); // Clear the trigger
  digitalWrite (triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite (triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite (triggerPin, LOW);
  pinMode (echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn (echoPin, HIGH);
}

bool sonarOn(){
  pinMode(LED, OUTPUT);

  int dt = 25;

  while(objectFound == false)
  {
    int cm = 0.017 * readUltrasonicDistance(trig, echo);

    Serial.print(cm);
    Serial.println("cm");

    
      digitalWrite(RIGHT_BACKWARD, 0);
      digitalWrite(RIGHT_FORWARD, 1); 
    
      digitalWrite(LEFT_BACKWARD, 0);
      digitalWrite(LEFT_FORWARD,1);

    if(cm <= dt)
    {
      Serial.println("Motor stop, put off fire");
      display.clearDisplay();
      display.setCursor(0,5);
      display.println("Object in front");
      display.println();
      display.print("Sprinkler activated");
      display.display();
      delay(10);
      objectFound == true;
      
      // delay(10000);

      
      //delay(1000);
      
      //digitalWrite(LED, 0);
       
      digitalWrite(RIGHT_BACKWARD, 1);
      digitalWrite(RIGHT_FORWARD, 1); 
    
      digitalWrite(LEFT_BACKWARD, 1);
      digitalWrite(LEFT_FORWARD,1);
     
      digitalWrite(LED, 1);
      delay(500);
      for(int i = 0; i<1;i++){
        for (pos = 0; pos <= 100; pos += 1) { // anti clockwise
          pump_servo.write(pos); 
          //fireDetected = pos;    
          delay(30);
        }
        for (pos = 100; pos >= 0; pos -= 1) { // anti clockwise
          pump_servo.write(pos); 
          //fireDetected = pos;    
          delay(30);
        }
      }
      digitalWrite(LED, 0);
      
      return true;
    }
      
  }
  return false;
  
    
}

int fireDetect()
{
  int ir1 = analogRead(IR_1);
  int ir2 = analogRead(IR_2);

    Serial.print("IR1 = ");
    Serial.print(ir1);
    Serial.print("  IR2 = ");
    Serial.println(ir2);

    display.clearDisplay();
    display.display();
    display.setCursor(0,5);
    display.print("IR1 = ");
    display.println(ir1);
    display.println();
    display.print("IR2 = ");
    display.println(ir2);
    display.display();
    delay(10);
  
}


bool servoRotate()
{
  
  for (pos = 0; pos <= 180; pos += 1) { // anti clockwise
    irServo.write(pos); 
    fireDetected = pos;    
    delay(10);

    ir1 = analogRead (IR_1);
    ir2 = analogRead (IR_2);

  //int Fire = fireDetect();
    
    if(ir1 <= firereading || ir2 <= firereading)
  { 
    irServo.write(0);
    Serial.print("Servo position = ");
     Serial.print(fireDetected);
    Serial.print("  IR1 = ");
    Serial.print(ir1);
    Serial.print("  IR2 = ");
    Serial.println(ir2);

    display.clearDisplay();
    display.setCursor(0,5);
    display.println("Fire detected at: ");
    display.println();
    display.print("Servo position = ");
    display.println(fireDetected);
    display.print("IR1 = ");
    display.println(ir1);
    display.println();
    display.print("IR2 = ");
    display.println(ir2);
    display.display();
    
    delay(500);   
     //digitalWrite(LEFT_FORWARD, 1);
     //delay(5000);
     return true;
  }
   
  }

  for (pos = 180; pos >= 0; pos -= 1) { // clockwise
    irServo.write(pos); 
    fireDetected = pos;    
    delay(10);
    
    ir1 = analogRead (IR_1);
    ir2 = analogRead (IR_2);

  //int Fire = fireDetect();
    
  if(ir1 <= firereading || ir2 <= firereading)
  {
    irServo.write(0);
    Serial.print("Servo position = ");
    Serial.print(fireDetected);
    Serial.print("  IR1 = ");
    Serial.print(ir1);
    Serial.print("  IR2 = ");
    Serial.println(ir2);

    display.clearDisplay();
    display.setCursor(0,5);
    display.println("Fire detected at: ");
    display.println();
    display.print("Servo position = ");
    display.println(fireDetected);
    display.print("IR1 = ");
    display.println(ir1);
    display.println();
    display.print("IR2 = ");
    display.println(ir2);
    display.display();
    
    delay(500);   
     //digitalWrite(LEFT_FORWARD, 1);
     //delay(5000);
     return true;
  }
   
  }
  
  return false;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);

  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(PWM,OUTPUT);

  analogWrite(PWM,200); 

  irServo.attach(IRservo_pin);
  irServo.write(0);

  pump_servo.attach(PumpServo_pin);
  pump_servo.write(0);

}

void loop() 
{
//    display.clearDisplay();
//    display.display();
//    delay(10);
    
//    display.setCursor(0,5);
//    display.print("Servo position = ");
//    display.print(fireDetected);
//    display.print("  IR1 = ");
//    display.print(ir1);
//    display.print("  IR2 = ");
//    display.println(ir2);
//    display.display();
//    delay(1000);
  
    digitalWrite(LEFT_FORWARD, 0);
  digitalWrite(LEFT_BACKWARD, 0);
  digitalWrite(RIGHT_FORWARD, 0);
  digitalWrite(RIGHT_BACKWARD, 0);
  
  bool gotFire = servoRotate();
  delay(100);

  while(gotFire == true)
  {

    if(ir1 <= firereading)
    {
      if(fireDetected <= 30)
      {
        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(1000);
        bool inFront = sonarOn();
        gotFire = false;
      }
      else if(fireDetected >= 31 && fireDetected <= 59)
      {
        digitalWrite(LEFT_FORWARD, 0);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 0);
        delay(300);
        fireDetected = 0;    
        
      }
      else if(fireDetected >= 60 && fireDetected <= 120)
      {
        digitalWrite(LEFT_FORWARD, 0);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 0);
        delay(550);
        fireDetected = 0;    
        
      }
      else if(fireDetected >= 121 && fireDetected <= 150)
      {
        digitalWrite(LEFT_FORWARD, 0);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 0);
        delay(800);
        fireDetected = 0;    
        
      }
      else if(fireDetected >= 151 && fireDetected <= 180)
      {
        digitalWrite(LEFT_FORWARD, 0);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 0);
        delay(1000);
        fireDetected = 0;    
        
      }
      else
      {
        //fireDetected = 0;
        break;
      }
    }

    else if(ir2 <= firereading)
    {
      if(fireDetected <= 30)
      {
//        digitalWrite(LEFT_FORWARD, 1);
//        digitalWrite(LEFT_BACKWARD, 1);
//        digitalWrite(RIGHT_FORWARD, 1);
//        digitalWrite(RIGHT_BACKWARD, 1);
//        delay(1000);
//        bool inFront = sonarOn();
//        gotFire = false;

        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 0);
        digitalWrite(RIGHT_FORWARD, 0);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(1000);
//        bool inFront = sonarOn();
//        gotFire = false; 
        fireDetected = 180; 

      }
      else if(fireDetected >= 31 && fireDetected <= 59)
      {
        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 0);
        digitalWrite(RIGHT_FORWARD, 0);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(800);
        fireDetected = 180;    
        
      }
      else if(fireDetected >= 60 && fireDetected <= 120)
      {
        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 0);
        digitalWrite(RIGHT_FORWARD, 0);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(550);
        fireDetected = 180;    
        
      }
      else if(fireDetected >= 121 && fireDetected <= 150)
      {
        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 0);
        digitalWrite(RIGHT_FORWARD, 0);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(300);
        fireDetected = 180;    
        
      }
      else if(fireDetected >= 151 && fireDetected <= 180)
      {
//        digitalWrite(LEFT_FORWARD, 1);
//        digitalWrite(LEFT_BACKWARD, 0);
//        digitalWrite(RIGHT_FORWARD, 0);
//        digitalWrite(RIGHT_BACKWARD, 1);
//        delay(200); 

        digitalWrite(LEFT_FORWARD, 1);
        digitalWrite(LEFT_BACKWARD, 1);
        digitalWrite(RIGHT_FORWARD, 1);
        digitalWrite(RIGHT_BACKWARD, 1);
        delay(1000);
//        fireDetected = 0;  
        bool inFront = sonarOn();
        gotFire = false;
        
      }
      else
      {
        //fireDetected = 0;
        break;
      }
      
    }
  }
  
    int fire = fireDetect();
    delay(10);
 
}
