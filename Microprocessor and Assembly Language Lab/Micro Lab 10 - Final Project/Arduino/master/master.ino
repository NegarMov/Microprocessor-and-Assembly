#include <SPI.h>
#include <Servo.h>

const int tempPin = A0;
const int Slave1_SS = 15;
const int Slave2_SS = 16;
int Vin;  // Variable to read the value from Arduino A0

float Temperature; // variable that receives the converted voltage
char TransferedData;
int mystatus = 0;
int servodegree;
int progress = 0;

Servo myservo1;
Servo myservo2;
Servo myservo3;

void setup (void) {
  //spi setup
  SPI.begin ();
  pinMode(Slave1_SS, OUTPUT);
  pinMode(Slave2_SS, OUTPUT);
  digitalWrite(Slave1_SS, HIGH);
  digitalWrite(Slave2_SS, HIGH);
  
  //read temp at start
  Vin = analogRead(tempPin);
  servodegree = 180;
  Temperature = (487.0 * Vin) / 1024; // Convert the read value into a voltage

  // serial setup
  Serial.begin(9600);
  Serial.println("MASTER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  
  //servo
  myservo1.attach(2);
  myservo1.write(180);
  myservo2.attach(3);
  myservo2.write(180);
  myservo3.attach(4);
  myservo3.write(180);
  
  //buzeer
  pinMode(0, OUTPUT);
}


void checkemergency() {
  if (Temperature > 120) {
    mystatus = 1;
    digitalWrite(0, HIGH);
    servodegree = 0;
    myservo1.write(0);
    myservo2.write(0);
    myservo3.write(0);
  }
  else {
    if (Temperature > 90 && Temperature < 120){
      digitalWrite(0, LOW);
      servodegree = 60;
      myservo1.write(60);
      myservo2.write(60);
      myservo3.write(60);
      mystatus = 0;
      progress++;
    } else if (Temperature > 60 && Temperature < 90) {
        digitalWrite(0, LOW);
        servodegree = 120;
        myservo1.write(120);
        myservo2.write(120);
        myservo3.write(120);
        mystatus = 0;
    } else {
        digitalWrite(0, LOW);
        servodegree = 180;
        myservo1.write(180);
        myservo2.write(180);
        myservo3.write(180);
        mystatus = 0;
    }
  }
}

void loop (void) {
  Vin = analogRead(tempPin);
  Temperature = (487.0 * Vin) / 1024;
  
  if (servodegree > 0) {
    checkemergency();
    
    //-----------------------------send status---------------------------------------
    /**** First Slave Data ****/
    digitalWrite(Slave1_SS, LOW);
    if (mystatus == 1) {
      for (const char * p = "emergency\r" ; TransferedData = *p; p++) {
        SPI.transfer(TransferedData);
        //Serial.println(TransferedData);
      }
    }
    else {
      for (const char * p = "safe\r" ; TransferedData = *p; p++) {
        SPI.transfer(TransferedData);
        //Serial.println(TransferedData);
      }
    }
    digitalWrite(Slave1_SS, HIGH);
    delay(20);
    
    /**** Second Slave Data ****/
    digitalWrite(Slave2_SS, LOW);
    if (mystatus == 1) {
      for (const char * p = "emergency\r" ; TransferedData = *p; p++) {
        SPI.transfer(TransferedData);
        //Serial.println(TransferedData);
      }
    }
    else {
      for (const char * p = "safe\r" ; TransferedData = *p; p++) {
        SPI.transfer(TransferedData);
        //Serial.println(TransferedData);
      }
    }
    digitalWrite(Slave2_SS, HIGH);
    delay(20);
    //-----------------------------send status---------------------------------------
  }
  if (servodegree == 0) {
    checkemergency();
  }
  
  Serial.print("Temperature: ");
  Serial.println(Temperature);
  Serial.print("Progress: ");
  Serial.println(progress);
  Serial.println("");
  
  delay(200);
}
