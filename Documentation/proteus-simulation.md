# Proteus simulation guide

## Core components

- STM32F103C8T6 / Blue Pill model
- Variable DC source for battery input
- 100 kOhm / 22 kOhm divider on PA0
- Adjustable analog source for current emulation on PA1
- LM35 temperature sensor on PA2
- NPN transistor relay driver on PB0
- Flyback diode across relay coil
- Green, orange and red LEDs on PB12/PB13/PB14
- Buzzer on PB1
- Virtual Terminal connected to USART1 TX/RX

## Battery-voltage channel

```text
VBAT ---- 100k ----+---- PA0
                   |
                  22k
                   |
                  GND
```

The divider ratio is 22 / (100 + 22), so 16 V produces about 2.89 V at the ADC input.

## Current channel for first simulation

Use an adjustable DC source on PA1 to emulate a sensor with 0.5 V/A:

| PA1 voltage | Simulated current |
|---:|---:|
| 0.0 V | 0 A |
| 0.5 V | 1 A |
| 1.0 V | 2 A |
| 1.5 V | 3 A |
| 2.0 V | 4 A |
| 2.5 V | 5 A |
| 3.0 V | 6 A |

For a physical prototype, replace this source with a proper current-sense amplifier and shunt resistor sized for the real load.

## Temperature channel

LM35 output is approximately 10 mV/C. Connect its output to PA2.

## Relay driver

```text
PB0 -- base resistor --> NPN transistor --> relay coil
                               |              |
                              GND       flyback diode
```

Use a de-energized = disconnected topology for fail-safe behavior.

## UART

- PA9 TX -> Virtual Terminal RX
- PA10 RX <- Virtual Terminal TX
- 115200 baud, 8N1

## Fault injection

Run the scenarios from `Tests/test_matrix.csv` by changing the three analog input values. Verify the state LEDs, relay state and UART telemetry against the expected results.
