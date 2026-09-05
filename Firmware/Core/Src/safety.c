#include "safety.h"
#include "sensors.h"
#include "app_config.h"
#include "main.h"
#include <stdbool.h>

static SafetyStatus_t status;
static uint8_t faultCounter = 0U;
static bool faultLatched = false;

static void SetRelay(bool enabled)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, enabled ? GPIO_PIN_SET : GPIO_PIN_RESET);
    status.relayEnabled = enabled ? 1U : 0U;
}

static void SetIndicators(SystemState_t state)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, state == STATE_NORMAL ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, state == STATE_WARNING ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,
                      (state == STATE_FAULT || state == STATE_SHUTDOWN) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,
                      (state == STATE_FAULT || state == STATE_SHUTDOWN) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static FaultCode_t DetectFault(const SensorData_t *d, bool criticalOnly)
{
    if (!d->valid) return FAULT_SENSOR;

    if (criticalOnly)
    {
        if (d->batteryVoltage <= VBAT_CRITICAL_LOW) return FAULT_UNDERVOLTAGE;
        if (d->batteryVoltage >= VBAT_CRITICAL_HIGH) return FAULT_OVERVOLTAGE;
        if (d->current >= CURRENT_CRITICAL) return FAULT_OVERCURRENT;
        if (d->temperature >= TEMP_CRITICAL) return FAULT_OVERTEMPERATURE;
    }
    else
    {
        if (d->batteryVoltage <= VBAT_FAULT_LOW) return FAULT_UNDERVOLTAGE;
        if (d->batteryVoltage >= VBAT_FAULT_HIGH) return FAULT_OVERVOLTAGE;
        if (d->current >= CURRENT_FAULT) return FAULT_OVERCURRENT;
        if (d->temperature >= TEMP_FAULT) return FAULT_OVERTEMPERATURE;
    }

    return FAULT_NONE;
}

static bool WarningActive(const SensorData_t *d)
{
    return (d->batteryVoltage <= VBAT_WARNING_LOW) ||
           (d->batteryVoltage >= VBAT_WARNING_HIGH) ||
           (d->current >= CURRENT_WARNING) ||
           (d->temperature >= TEMP_WARNING);
}

static bool RecoveryConditionsMet(const SensorData_t *d)
{
    return d->valid &&
           d->batteryVoltage > VBAT_RECOVERY_LOW &&
           d->batteryVoltage < VBAT_RECOVERY_HIGH &&
           d->current < CURRENT_RECOVERY &&
           d->temperature < TEMP_RECOVERY;
}

void Safety_Init(void)
{
    status.state = STATE_INIT;
    status.fault = FAULT_NONE;
    status.relayEnabled = 0U;
    faultCounter = 0U;
    faultLatched = false;
    SetRelay(false);
    SetIndicators(STATE_INIT);
}

void Safety_Update(void)
{
    SensorData_t d = Sensors_GetData();

    if (!Sensors_IsReady())
    {
        status.state = STATE_INIT;
        status.fault = d.valid ? FAULT_NONE : FAULT_SENSOR;
        SetRelay(false);
        SetIndicators(status.state);
        return;
    }

    FaultCode_t critical = DetectFault(&d, true);
    if (critical != FAULT_NONE)
    {
        status.state = STATE_SHUTDOWN;
        status.fault = critical;
        faultLatched = true;
        SetRelay(false);
        SetIndicators(status.state);
        return;
    }

    FaultCode_t fault = DetectFault(&d, false);
    if (fault != FAULT_NONE)
    {
        if (faultCounter < FAULT_QUALIFICATION_COUNT) faultCounter++;
        if (faultCounter >= FAULT_QUALIFICATION_COUNT)
        {
            status.state = STATE_FAULT;
            status.fault = fault;
            faultLatched = true;
            SetRelay(false);
            SetIndicators(status.state);
            return;
        }
    }
    else
    {
        faultCounter = 0U;
    }

    if (faultLatched)
    {
        status.state = STATE_FAULT;
        SetRelay(false);
        SetIndicators(status.state);
        return;
    }

    if (WarningActive(&d))
    {
        status.state = STATE_WARNING;
        status.fault = FAULT_NONE;
        SetRelay(true);
    }
    else
    {
        status.state = STATE_NORMAL;
        status.fault = FAULT_NONE;
        SetRelay(true);
    }

    SetIndicators(status.state);
}

void Safety_ResetLatchedFault(void)
{
    SensorData_t d = Sensors_GetData();
    if (RecoveryConditionsMet(&d))
    {
        faultLatched = false;
        faultCounter = 0U;
        status.fault = FAULT_NONE;
        status.state = STATE_NORMAL;
    }
}

SafetyStatus_t Safety_GetStatus(void)
{
    return status;
}
