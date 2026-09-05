# Roadmap

## V1 - Core protection ECU
- ADC + DMA acquisition
- Filtering
- Safety state machine
- Relay/LED/buzzer outputs
- UART telemetry
- Python dashboard
- Proteus fault injection

## V2 - Automotive communication
- CAN 2.0B communication
- Suggested IDs: 0x100 voltage, 0x101 current, 0x102 temperature, 0x200 state, 0x201 DTC
- External CAN transceiver for physical hardware

## V3 - RTOS
- FreeRTOS SensorTask, SafetyTask, CANTask, TelemetryTask and DiagnosticTask
- Queues for sensor snapshots
- Mutex-protected diagnostics

## V4 - Diagnostics and persistence
- DTC history
- Reset-cause logging
- Flash persistence with wear management
- UART/CAN diagnostic commands

## V5 - Safety engineering package
- Detailed software requirements
- FMEA/FMEDA-inspired analysis
- Requirements-to-test traceability
- Timing and fault-injection reports
