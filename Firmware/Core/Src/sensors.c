#include "sensors.h"
#include "app_config.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

static uint16_t adcBuffer[3];
static SensorData_t data;
static uint8_t validSampleCount = 0U;
static bool filterInitialized = false;

static float ADC_ToVoltage(uint16_t adc)
{
    return ((float)adc * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;
}

static float LowPass(float input, float previous)
{
    return (FILTER_ALPHA * input) + ((1.0f - FILTER_ALPHA) * previous);
}

static bool Plausible(float vbat, float current, float temperature)
{
    return (vbat >= 0.0f && vbat <= 20.0f) &&
           (current >= 0.0f && current <= 7.0f) &&
           (temperature >= -20.0f && temperature <= 150.0f);
}

void Sensors_Init(void)
{
    data.batteryVoltage = 0.0f;
    data.current = 0.0f;
    data.temperature = 0.0f;
    data.valid = false;
    validSampleCount = 0U;
    filterInitialized = false;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcBuffer, 3U);
}

void Sensors_Update(void)
{
    const float dividerRatio = (VOLTAGE_DIVIDER_R1 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2;
    float rawVbat = ADC_ToVoltage(adcBuffer[0]) * dividerRatio;
    float rawCurrent = ADC_ToVoltage(adcBuffer[1]) / CURRENT_SENSOR_GAIN;
    float rawTemperature = ADC_ToVoltage(adcBuffer[2]) * 100.0f;

    if (!filterInitialized)
    {
        data.batteryVoltage = rawVbat;
        data.current = rawCurrent;
        data.temperature = rawTemperature;
        filterInitialized = true;
    }
    else
    {
        data.batteryVoltage = LowPass(rawVbat, data.batteryVoltage);
        data.current = LowPass(rawCurrent, data.current);
        data.temperature = LowPass(rawTemperature, data.temperature);
    }

    data.valid = Plausible(data.batteryVoltage, data.current, data.temperature);

    if (data.valid && validSampleCount < STARTUP_VALID_SAMPLES)
    {
        validSampleCount++;
    }
    else if (!data.valid)
    {
        validSampleCount = 0U;
    }
}

SensorData_t Sensors_GetData(void)
{
    return data;
}

bool Sensors_IsReady(void)
{
    return data.valid && (validSampleCount >= STARTUP_VALID_SAMPLES);
}
