# STM32 Smart Power Monitoring & Protection ECU

🇬🇧 **English** | [🇫🇷 Français](README.fr.md)

![Architecture](docs/images/architecture.svg)

Embedded power monitoring and protection ECU based on the STM32F103C8T6. The project monitors battery voltage, load current and temperature, detects abnormal conditions, controls a fail-safe relay, reports diagnostics over UART and provides a Python monitoring dashboard.

## Main features

- STM32F103C8T6 / Blue Pill target
- 3-channel ADC acquisition with DMA
- Battery voltage, current and LM35 temperature sensing
- First-order low-pass filtering
- Safety state machine: INIT, NORMAL, WARNING, FAULT, SHUTDOWN
- Fault qualification and hysteresis
- Fail-safe relay output, status LEDs and buzzer
- UART telemetry at 115200 baud
- Python serial dashboard and CSV logger
- Proteus-oriented simulation documentation
- Requirements, DTCs and verification test matrix
- Roadmap for CAN, FreeRTOS and AUTOSAR-inspired layering

## System visuals

### Safety state machine

![Safety state machine](docs/images/state-machine.svg)

### Proteus wiring overview

![Proteus wiring overview](docs/images/proteus-wiring.svg)

## Repository structure

```text
Firmware/
  Core/Inc/
  Core/Src/
Dashboard/
Documentation/
Tests/
docs/images/
```

## Hardware pinout

| Signal | STM32 pin |
|---|---|
| Battery voltage ADC | PA0 / ADC1_IN0 |
| Current ADC | PA1 / ADC1_IN1 |
| Temperature ADC | PA2 / ADC1_IN2 |
| UART TX | PA9 |
| UART RX | PA10 |
| Relay | PB0 |
| Buzzer | PB1 |
| Green LED | PB12 |
| Orange LED | PB13 |
| Red LED | PB14 |

## Safety thresholds

The default thresholds are demonstration values and must be adapted to the real electrical system.

| Signal | Warning | Fault | Shutdown |
|---|---:|---:|---:|
| Battery high | 14.3 V | 15.0 V | 16.0 V |
| Battery low | 11.0 V | 10.0 V | 9.0 V |
| Current | 3.5 A | 4.5 A | 5.5 A |
| Temperature | 60 C | 80 C | 90 C |

## Quick start

1. Create an STM32CubeIDE project for STM32F103C8T6.
2. Configure ADC1 channels PA0/PA1/PA2 with DMA in circular mode.
3. Configure USART1 at 115200 8N1.
4. Configure PB0, PB1, PB12, PB13 and PB14 as GPIO outputs.
5. Copy the files from `Firmware/Core/Inc` and `Firmware/Core/Src` into the generated CubeIDE project.
6. Call `HAL_ADCEx_Calibration_Start(&hadc1)` before `Sensors_Init()`.
7. Enable float formatting for printf if needed using `-u _printf_float`.
8. Install the dashboard dependencies with `pip install -r Dashboard/requirements.txt`.
9. Run `python Dashboard/main.py --port COM4` and replace COM4 with your serial port.

## Telemetry frame

```text
VBAT=12.42,CURRENT=1.35,TEMP=28.10,STATE=1,FAULT=0,RELAY=1
```

## Documentation

- [Architecture](Documentation/architecture.md)
- [Proteus simulation wiring](Documentation/proteus-simulation.md)
- [Requirements and traceability](Documentation/requirements.md)
- [Verification plan](Documentation/test-plan.md)
- [Roadmap](Documentation/roadmap.md)

## Project goal

This repository demonstrates embedded C development, STM32 peripherals, diagnostics, fault handling, functional-safety-oriented software design, simulation and test automation in one recruiter-friendly project.

## Disclaimer

This is an engineering demonstration project. Thresholds and protection circuitry must be validated before use on real power electronics, batteries or safety-critical systems.
