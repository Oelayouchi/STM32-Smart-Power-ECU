# Software architecture

The project follows a simple layered architecture inspired by automotive embedded software practices.

```text
Application / Scheduler
        |
Safety Manager
        |
Sensor Services ---- Telemetry Service
        |
STM32 HAL / DMA / UART / GPIO
        |
STM32F103C8T6 Hardware
```

## Sensor service

Responsibilities:
- start ADC DMA acquisition
- convert ADC counts to engineering units
- apply low-pass filtering
- perform signal plausibility checks
- expose a validated sensor snapshot

## Safety manager

Responsibilities:
- keep the load disconnected during startup
- detect warning, fault and shutdown conditions
- qualify non-critical faults for multiple samples
- latch confirmed faults
- apply recovery hysteresis before reset
- drive relay, buzzer and state LEDs

## Telemetry service

Responsibilities:
- serialize measurements and safety status
- send frames over USART1
- accept diagnostic commands such as `RESET_FAULT`

## Scheduling

The initial implementation uses a cooperative time-triggered scheduler:
- sensor task: 10 ms
- safety task: 20 ms
- telemetry task: 100 ms

A later version can migrate these functions to FreeRTOS tasks and queues.
