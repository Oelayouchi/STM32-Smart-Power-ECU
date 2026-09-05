# STM32CubeIDE integration

## Target

STM32F103C8T6, 72 MHz system clock.

## CubeMX configuration

### Clock
- HSE: 8 MHz crystal
- PLL source: HSE
- PLL multiplier: x9
- SYSCLK: 72 MHz

### ADC1
- Scan Conversion Mode: enabled
- Continuous Conversion Mode: enabled
- Number of conversions: 3
- Rank 1: ADC1_IN0 / PA0
- Rank 2: ADC1_IN1 / PA1
- Rank 3: ADC1_IN2 / PA2
- DMA: DMA1 Channel1, Peripheral-to-Memory, Circular, Half Word

### USART1
- PA9 TX
- PA10 RX
- 115200 baud
- 8 data bits, no parity, 1 stop bit

### GPIO outputs
- PB0 relay
- PB1 buzzer
- PB12 green LED
- PB13 orange LED
- PB14 red LED

## main.c application integration

Add the headers:

```c
#include "sensors.h"
#include "safety.h"
#include "telemetry.h"
#include "app_config.h"
```

After CubeMX peripheral initialization:

```c
HAL_ADCEx_Calibration_Start(&hadc1);
Sensors_Init();
Safety_Init();

uint32_t sensorTick = HAL_GetTick();
uint32_t safetyTick = HAL_GetTick();
uint32_t telemetryTick = HAL_GetTick();
```

Inside the infinite loop:

```c
while (1)
{
    uint32_t now = HAL_GetTick();

    if ((now - sensorTick) >= SENSOR_PERIOD_MS)
    {
        sensorTick = now;
        Sensors_Update();
    }

    if ((now - safetyTick) >= SAFETY_PERIOD_MS)
    {
        safetyTick = now;
        Safety_Update();
    }

    if ((now - telemetryTick) >= TELEMETRY_PERIOD_MS)
    {
        telemetryTick = now;
        Telemetry_Send();
    }
}
```

## Important build note

STM32 newlib-nano may omit floating-point support in printf by default. Add the linker option:

```text
-u _printf_float
```

## Fail-safe behavior

The relay is kept OFF during reset, startup validation, FAULT and SHUTDOWN. In real hardware, wire the relay or MOSFET stage so that a de-energized control output disconnects the protected load.
