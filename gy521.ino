#include <Wire.h> // This library allows you to communicate with I2C devices.

#define ventanaMuestras 10
#define multiplicadorMovimiento 1000

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
byte muestras=0;
int16_t ax_anterior,ay_anterior,az_anterior;
unsigned long sumatoria_x=0;
unsigned long sumatoria_y=0;
unsigned long sumatoria_z=0;
double parcial_xy=0;
double parcial_xyz=0;
unsigned long xyzMagnitud=0;

void setup(){
  Serial.begin(9600);
  // configura elacelerometro
  configAccel();
  //Toma muestras iniciales solo de aceleracion
  getData();

}

void loop(){
  // tomo el valor anterior de cada eje que es el ultimo leido
  ax_anterior=accelerometer_x;            
  ay_anterior=accelerometer_y;            
  az_anterior=accelerometer_z;

  // renuevo los datos
  getData();

 

  /*Serial.print("aX = ");*/ Serial.print(accelerometer_x);Serial.print(",");
  /*Serial.print(" | aY = ");*/ Serial.print(accelerometer_y);Serial.print(",");
  /*Serial.print(" | aZ = ");*/ Serial.print(accelerometer_z);
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  //Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  //Serial.print(" | gX = "); Serial.print(gyro_x);
  //Serial.print(" | gY = "); Serial.print(gyro_y);
  //Serial.print(" | gZ = "); Serial.print(gyro_z);
  Serial.println();  

  //Diferencias sucesivas
  //sumatoria_x+=abs(accelerometer_x-ax_anterior);
  //sumatoria_y+=abs(accelerometer_y-ay_anterior);
  //sumatoria_z+=abs(accelerometer_z-az_anterior);
 /*
  //Ventana de analisis cada X muestras
  if(muestras==ventanaMuestras) {
    xyzMagnitud=0;
    parcial_xy=(sumatoria_x/100)*(sumatoria_y/100);
    parcial_xyz= parcial_xy*(sumatoria_z/100);
    xyzMagnitud=(long)parcial_xyz;
    Serial.println(xyzMagnitud);
    if(xyzMagnitud<ventanaMuestras*multiplicadorMovimiento){
      Serial.println("QUIETO");          
    }
    else {
      Serial.println("SE MOVIO");
    }
    sumatoria_x=sumatoria_y=sumatoria_z=0;
    muestras=0;
  }
  muestras++;
 */
}

void configAccel(){
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void getData(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
 
  // «Wire.read()<<8 | Wire.read();» means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  // delay
  delay(100);
}
