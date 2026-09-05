#include "telemetry.h"
#include "sensors.h"
#include "safety.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

void Telemetry_Send(void)
{
    char buffer[160];
    SensorData_t sensors = Sensors_GetData();
    SafetyStatus_t safety = Safety_GetStatus();

    int len = snprintf(buffer, sizeof(buffer),
                       "VBAT=%.2f,CURRENT=%.2f,TEMP=%.2f,STATE=%d,FAULT=%d,RELAY=%d\r\n",
                       sensors.batteryVoltage,
                       sensors.current,
                       sensors.temperature,
                       (int)safety.state,
                       (int)safety.fault,
                       (int)safety.relayEnabled);

    if (len > 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buffer, (uint16_t)len, 100U);
    }
}

void Telemetry_ProcessCommand(const char *command)
{
    if (command == NULL) return;

    if (strcmp(command, "RESET_FAULT") == 0)
    {
        Safety_ResetLatchedFault();
    }
}
