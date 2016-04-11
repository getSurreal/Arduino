#include "SHT2x.h"
#include <Wire.h>

SHT2x::SHT2x(){
  
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/
/*
boolean SHT2x::setResolution(uint8_t resolution){
  uint8_t user_data = getUserData(false);
  if(user_data == 0){  // if no data is returned
    return false;
  }
  
  if(resolution < 4){  // valid values are 0,1,2,3
//    user_data &= 0x3F; // resolution value is in the top two bits of the user register
    user_data |= (resolution << 6);    
  }
  
  Wire.beginTransmission(SHT2x_I2C_ADDRESS); 
  Wire.write(WRITE_USER_DATA);
  Wire.write(user_data);              
  Wire.endTransmission();     
  
  return true; 
}
*/

float SHT2x::getTemperatureC(){
	return (-46.85 + 175.72 / 65536.0 * (float)(readSensor(TRIG_T_MEASUREMENT_HM)));
}

float SHT2x::getTemperatureF(){
	return getTemperatureC() * 9.0 / 5.0 + 32.0;
}

float SHT2x::getHumidity(){
	return (-6.0 + 125.0 / 65536.0 * (float)(readSensor(TRIG_RH_MEASUREMENT_HM)));
}

void SHT2x::softReset(){
	  Wire.beginTransmission(SHT2x_I2C_ADDRESS);
	  Wire.write(SOFT_RESET); 
	  Wire.endTransmission(true);
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/

uint16_t SHT2x::readSensor(uint8_t command)
{
	uint8_t data[2] = {0};
	uint8_t checksum;
    uint16_t result;

    Wire.beginTransmission(SHT2x_I2C_ADDRESS);
    Wire.write(command);
    Wire.endTransmission(false);

    Wire.requestFrom(SHT2x_I2C_ADDRESS, 3);

	unsigned long startMillis = millis();  

	while(Wire.available() < 3) {
		if(millis() - startMillis >= 100){ // maximum duration 85ms (*datasheet)
		  return -1;
		}  
    }
	data[0] = Wire.read();
	data[1] = Wire.read();
	checksum = Wire.read();
	
	result = data[0] << 8;
	result |= data[1];
	result &= ~0x0003;   // clear the two low bits (status bits)
	if(!checkCRC(data, 2, checksum)){
	  	return -1;
	  }
	return result;
	
}

boolean SHT2x::checkCRC(uint8_t * data, uint8_t nbrOfBytes, uint8_t checksum){
    uint8_t crc = 0;
    uint8_t byteCtr = 0;
    //calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
        crc ^= (data[byteCtr]);
        for (uint8_t bit = 8; bit > 0; --bit) {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }

    //crc &= 0xff;    
    
    if (crc != checksum) return false;
    else return true;
}