#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float batteryVoltage;
    float current;
    float temperature;
    bool valid;
} SensorData_t;

void Sensors_Init(void);
void Sensors_Update(void);
SensorData_t Sensors_GetData(void);
bool Sensors_IsReady(void);

#endif
