# Cowtracker
Script and pinout for building a Arduino based tracking device recording motion and GPS data. Intended for use on grazig cattle

## General
Adafruit hardware is assembled combining a microprocessor including a SD card slot with an IMU (accelerometer and gyroscope) and GPS breakour board. The device is powered by an 3.7 V LiPo battery. The data obtained from the two sensors is written to an SD card in csv format. Three days of tracking produces approx. 270 MB at 20Hz reading frequency of the IMU and 0.5 HZ of the GPS breakout.

## Libraries
The following libraries are needed:
SD.h
SPI.h
Adafruit_GPS.h
Adafruit_MPU6050.h
Adafruit_Sensor.h
Wire.h

## Hardware documentaion

Microcontroller https://learn.adafruit.com/adafruit-feather-m0-adalogger
GPS breakout https://learn.adafruit.com/adafruit-ultimate-gps
IMU https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro

## Wiring


