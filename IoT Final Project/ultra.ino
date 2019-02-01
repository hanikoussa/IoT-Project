#include "NewPing.h"
#include "Servo.h"

#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 11
#define TxD 10
#define HCSR04_PIN_TRIG  3
#define HCSR04_PIN_ECHO 2
#define SERVO9G_PIN_SIG  4
const int servo9gRestPosition   = 0;  //Starting position
const int servo9gTargetPosition = 90; //Position when event is detected
SoftwareSerial blueToothSerial(RxD,TxD);
int state = 0;
char data=0;
bool gate= true;
bool red1= true;
bool red2= true;
 const int IR1 = A3;
 const int IR2 = A1;
 Servo servo9g;
NewPing hcsr04(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    pinMode(IR1,INPUT);
    pinMode(IR2,INPUT);
     servo9g.attach(SERVO9G_PIN_SIG);
    servo9g.write(servo9gRestPosition);
    delay(100);
    servo9g.detach();

    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    
    setupBlueToothConnection();

    
}

void loop() 
{

  int hcsr04Dist = hcsr04.ping_cm();
  
  delay(250);
    if (hcsr04Dist<10 && hcsr04Dist>0 && hcsr04Dist!=0)
    { //check for car approaching gate
        
        Serial.print(F("Distance: "));
        Serial.print(hcsr04Dist); 
        Serial.println(F("[cm]"));
        Serial.print("P2 Small: ");
        Serial.print(" ");
            if (digitalRead(IR1)==HIGH)
              {
                
                Serial.println("OCCUPIED");
                red1=false; //Send  car to second infrared sensor
               }
            else
            {
                Serial.println("FREE");
            }
         Serial.print("P1 Big : ");
         Serial.print(" ");
    
        if (digitalRead(IR2)==LOW)
        {
          Serial.println("OCCUPIED");
          stopCar6();
          red2=false;//send car back if both sensors are occupied
         }
        else
          {
          Serial.println("FREE");
          stopCar4();
          red1=false; 
          }
    
   if((digitalRead(IR1)==LOW) || (digitalRead(IR2)==HIGH)){  //if there is a free parking space enter parking lot
          servo9g.attach(SERVO9G_PIN_SIG);         // 1. attach the servo to correct pin to control it.
          servo9g.write(servo9gTargetPosition);// 2. turns servo to target position. Opens gate for car to enter
          Serial.println("GATE OPEN");
          delay(1000);
              if(red2)
              {
                  carStart2();
                  Serial.println("Going to P1");
      
              }
              else
              { 
                  carStart3();
                  Serial.println("Going to P2");
      
              }
    while(gate)
    { //keep gate open untill the car passed through
        delay(500);
        int hcsr04Dist = hcsr04.ping_cm();
        Serial.println("CAR IN FRONT OF GATE");
        Serial.print("Distance: ");
        Serial.print(hcsr04Dist);
        delay(500);
            if (!(hcsr04Dist<10 && hcsr04Dist>0 && hcsr04Dist!=0 && hcsr04Dist>100))
            { //when sensor senses car is no longer available closes the gate
                Serial.println("CLOSING GATE");
                delay(4000);
                Serial.println("Test");
                servo9g.attach(SERVO9G_PIN_SIG);
                servo9g.write(servo9gRestPosition);
        
                gate=false;
                delay(500);
                servo9g.detach();
            }
     } 
    while(red2)
      { //keep car moving towards sensor until it reaches parking space
        Serial.println("GUIDING CAR TO P1");
        delay(1000);
        if (digitalRead(IR2)==LOW)
        {
        red2=false;
        stopCar8(); 
          }
       }
        
    while(red1)
      {
          Serial.println("GUIDING CAR TO P2");
          delay(1000);
         if (digitalRead(IR1)==HIGH)
         {
        red1=false;
        stopCar5();
            }
       }
          
     
  
     delay(3000);    
  }
  else //no available spaces
  {
    carBack();
      while(gate)
      { //reverses car until it leaves the gate
        Serial.print("TEST BACKING UP NO AVAILABLE PARKING: ");
        int hcsr04Dist = hcsr04.ping_cm();
        Serial.println(hcsr04Dist);
        delay(500);
          if (!(hcsr04Dist<10 && hcsr04Dist>0 && hcsr04Dist!=0 && hcsr04Dist>100))
          {
            Serial.print("TEST NO CAR AFTER BACKING UP CLOSING GATE");
            Serial.println(hcsr04Dist);
            delay(2000);
            gate=false;
            stopCar8();
          }
      }
  } 

  }
  red1=true;
  red2=true;
  gate=true;
}

void setupBlueToothConnection()
{
 //  blueToothSerial.begin(9600);
    Serial.print("14CORE BLuetooth XBEE Test Code");
 Serial.print("Setting up Bluetooth link"); //For debugging, Comment this line if not required    
    blueToothSerial.begin(9600); //Set BluetoothBee BaudRate to default baud rate 38400
    delay(1000);
    sendBlueToothCommand("\r\n+STWMOD=1\r\n"); // Set as Master Mode
    sendBlueToothCommand("\r\n+STNA=The-Master\r\n");
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");  //Close the function
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");  //Open the function
    sendBlueToothCommand("\r\n+STPIN=0000\r\n"); //Set pin code 0000
    sendBlueToothCommand("\r\n+CONN=00,6A,8E,16,C4,A4\r\n");
    delay(2000); // This delay is required.
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    sendBlueToothCommand("\r\n+INPIN\r\n");
    //blueToothSerial.print("\r\n+INQ=1\r\n");
    delay(2000); // This delay is required.
    Serial.print("Setup complete");
 
}
 
void sendBlueToothCommand(char command[])
{
   
    char a;
    blueToothSerial.print(command);
    Serial.print(command);                          
    delay(3000);
 
    while(blueToothSerial.available())             
    {                                                 
       Serial.print(char(blueToothSerial.read()));    
    }                                                
}

void carBack(){
  blueToothSerial.write('1');
  Serial.println("1 Reverse Car");
  }

void stopCar8(){
  blueToothSerial.write('8');
  Serial.println("8 Stop Car");
  }

void stopCar4(){
  blueToothSerial.write('8');
  Serial.println("4 Stop Car");
  }

void stopCar5(){
  blueToothSerial.write('8');
  Serial.println("5 Stop Car");
  }

void stopCar6(){
  blueToothSerial.write('8');
  Serial.println("6 Stop Car");
  }

void carStart2(){
  blueToothSerial.write('2');
  Serial.println("2 Go!");
  }
void carStart3(){
  blueToothSerial.write('3');
  Serial.println("3 Go!");
  }
  
