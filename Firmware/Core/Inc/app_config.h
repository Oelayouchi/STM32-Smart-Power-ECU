#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define ADC_REFERENCE_VOLTAGE      3.3f
#define ADC_MAX_VALUE              4095.0f
#define VOLTAGE_DIVIDER_R1         100000.0f
#define VOLTAGE_DIVIDER_R2         22000.0f
#define CURRENT_SENSOR_GAIN        0.5f
#define FILTER_ALPHA               0.10f

#define VBAT_WARNING_LOW           11.0f
#define VBAT_WARNING_HIGH          14.3f
#define VBAT_FAULT_LOW             10.0f
#define VBAT_FAULT_HIGH            15.0f
#define VBAT_CRITICAL_LOW          9.0f
#define VBAT_CRITICAL_HIGH         16.0f
#define VBAT_RECOVERY_LOW          10.5f
#define VBAT_RECOVERY_HIGH         14.7f

#define CURRENT_WARNING            3.5f
#define CURRENT_FAULT              4.5f
#define CURRENT_CRITICAL           5.5f
#define CURRENT_RECOVERY           4.0f

#define TEMP_WARNING               60.0f
#define TEMP_FAULT                 80.0f
#define TEMP_CRITICAL              90.0f
#define TEMP_RECOVERY              75.0f

#define SENSOR_PERIOD_MS           10U
#define SAFETY_PERIOD_MS           20U
#define TELEMETRY_PERIOD_MS        100U
#define STARTUP_VALID_SAMPLES      10U
#define FAULT_QUALIFICATION_COUNT  5U

#endif
