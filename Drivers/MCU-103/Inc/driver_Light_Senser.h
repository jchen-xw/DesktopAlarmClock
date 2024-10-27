#ifndef __DRIVER_LIGHT_SENSER_H_
#define __DRIVER_LIGHT_SENSER_H_

#define LIGHT_SENSOR_ADC_TIMEOUT        50

void LightSenser_Init( void );

void LightSenser_GetValue( uint8_t *LightValue );


#endif
