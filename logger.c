#include <SD.h>
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int chipSelect = 4;

Adafruit_GPS GPS(&GPSSerial);

uint32_t timer = millis();//for writing data on SD card
uint32_t timer2 = millis();//for timestamping accelerometer data
uint32_t timer3 = millis();//for writing accelerometer data to paragraph string
String parag;
String header= "ax,ay,az,gx,gy,gz,temp,timer,date,timect,lat,long,fix,fix_qual,n_sat,angle,speed,altd";
Adafruit_MPU6050 mpu;

void setupSensor() {
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void setup() 
{
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
 if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");

    while (1);
  }
  Serial.println("card initialized.");
{
  Serial.begin(115200);

  GPS.begin(9600);

 GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

 

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  GPSSerial.println(PMTK_Q_RELEASE);

  while (!Serial) {
    delay(1); 
  } 
  while (!Serial)
    delay(10); 

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
 
  setupSensor();
}
File dataFile = SD.open("log.txt", FILE_WRITE);
 if (dataFile) {
 
dataFile.print(header);
dataFile.close();
 }
}

void loop()
{
 char c = GPS.read();

  if (GPSECHO)
    if (c) Serial.print(c);

  if (GPS.newNMEAreceived()) {
    
    Serial.print(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA())) 
      return; 
  }

  
  


  /* Get a new sensor event */ 
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);


timer2=millis();//timer2 to merge data of accel. to real time of GPS
  
  parag;
if ( timer3 > millis () ) timer3 = millis () ;
 if (millis() - timer3 > 50) { 
   timer3 = millis () ;
 parag=String(parag + '\n' +a.acceleration.x+',' + a.acceleration.y +',' + a.acceleration.z + ','+g.gyro.x+','+g.gyro.y+','+g.gyro.z+','+temp.temperature+','+timer2);//
 
 
 }


if ( timer > millis () ) timer = millis () ;
 if (millis() - timer > 2000) {
timer = millis(); // reset the timer

     

  File dataFile = SD.open("log.txt", FILE_WRITE);
 if (dataFile) {
 //time and date

dataFile.print(parag);dataFile.print(',');
 dataFile.print(GPS.day,DEC);dataFile.print('/');
  dataFile.print(GPS.month,DEC);dataFile.print('/');
  dataFile.print(GPS.year,DEC);
  dataFile.print(',');
   if (GPS.hour<10) {dataFile.print('0') ;}
  dataFile.print(GPS.hour, DEC);
  dataFile.print(':');
   if (GPS.minute<10) {dataFile.print('0') ;}
  dataFile.print(GPS.minute,DEC);
  dataFile.print(':');
   if (GPS.seconds<10) {dataFile.print('0') ;}
  dataFile.print(GPS.seconds,DEC);
   dataFile.print('.');
   if (GPS.milliseconds<10) {dataFile.print('00') ;}
   else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      dataFile.print("0");
    }
    dataFile.print(GPS.milliseconds);

  dataFile.print(',');
 //location
  if (GPS.fix){
     dataFile.print(GPS.latitude,8);dataFile.print(GPS.lat);dataFile.print(',');
     dataFile.print(GPS.longitude,8);dataFile.print(GPS.lon);dataFile.print(',');
     dataFile.print((int)GPS.fix);dataFile.print(',');
     dataFile.print((int)GPS.fixquality);dataFile.print(',');
     dataFile.print((int)GPS.satellites);dataFile.print(',');
     dataFile.print(GPS.angle);dataFile.print(',');
      dataFile.print(GPS.speed);dataFile.print(',');
     dataFile.print(GPS.altitude);//check if println is necessary
    
    dataFile.close();
    parag=String();
     
  }
  else{
     
     dataFile.close();
     parag=String();
     
  }
  }
 

 else {
    Serial.println("error opening datalog");
    delay (1000) ;
return ;
  }
   }
}
