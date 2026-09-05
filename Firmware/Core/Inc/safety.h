#ifndef SAFETY_H
#define SAFETY_H

#include <stdint.h>

typedef enum
{
    STATE_INIT = 0,
    STATE_NORMAL,
    STATE_WARNING,
    STATE_FAULT,
    STATE_SHUTDOWN
} SystemState_t;

typedef enum
{
    FAULT_NONE = 0,
    FAULT_UNDERVOLTAGE,
    FAULT_OVERVOLTAGE,
    FAULT_OVERCURRENT,
    FAULT_OVERTEMPERATURE,
    FAULT_SENSOR
} FaultCode_t;

typedef struct
{
    SystemState_t state;
    FaultCode_t fault;
    uint8_t relayEnabled;
} SafetyStatus_t;

void Safety_Init(void);
void Safety_Update(void);
void Safety_ResetLatchedFault(void);
SafetyStatus_t Safety_GetStatus(void);

#endif
