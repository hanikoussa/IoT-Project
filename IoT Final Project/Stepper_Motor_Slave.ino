
int Pin1 = 10; 
int Pin2 = 11; 
int Pin3 = 12; 
int Pin4 = 13; 
int _step = 0;  
boolean dir = false;// false=clockwise, true=counter clockwise
int count=0;
#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 6
#define TxD 5
SoftwareSerial blueToothSerial(RxD,TxD);
char data=0;

char state = '0'; //for reading bluetooth input
int vool=0;
void setup() {
  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
  pinMode(Pin4, OUTPUT);
  Serial.begin(9600);          //Serial port for debugging, 
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  //pinMode(13,OUTPUT);
  
  setupBlueToothConnection();

}

void loop() {
  if(blueToothSerial.available()>0)             
    {                                                      
       state = (char(blueToothSerial.read()));   
    Serial.println(state);
    }
  if (state == '2' || state == '3'|| state == '1')
  {
    dir=false;
  if(state=='1')
  dir=true;
   
    while(state == '2' || state == '3' || state == '1'){
    switch(_step){ 
   case 0: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, 250); 
   break;  
   case 1: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, 250); 
     digitalWrite(Pin4, 250); 
   break;  
   case 2: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, 250); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 3: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, 250); 
     digitalWrite(Pin3, 250); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 4: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, 250); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 5: 
     digitalWrite(Pin1, 250);  
     digitalWrite(Pin2, 250); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
     case 6: 
     digitalWrite(Pin1, 250);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 7: 
     digitalWrite(Pin1, 250);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, 250); 
   break;  
   default: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
 } 
 if(dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 } 
 delay(1);

if (blueToothSerial.available() > 0) { // Checks whether data is comming from the serial port
    char vool= 0;
 state = (char(blueToothSerial.read()));
 Serial.print("State: ");
 Serial.println(state);
 if (state == '8'){
  Serial.print("ana fetet");
  stop_motor();
  
 break;}
  }
 }
 
 }
  
    }




void setupBlueToothConnection()
{
  Serial.print("Step-Motor Via Bluetooth Test:");
  Serial.print("Setting up Bluetooth link"); //For debugging, Comment this line if not required    
    blueToothSerial.begin(9600); //Set BluetoothBee BaudRate to default baud rate 38400
    delay(1000);
    sendBlueToothCommand("\r\n+STWMOD=0\r\n"); // Set as Slave Mode
    sendBlueToothCommand("\r\n+STNA=The-Slave\r\n");
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");  //Close the function
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");  //Open the function
    sendBlueToothCommand("\r\n+STPIN=0000\r\n"); //Set pin code 0000
    delay(2000); // This delay is required.
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    sendBlueToothCommand("\r\n+INPIN\r\n");
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

void stop_motor(){
   digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
}
