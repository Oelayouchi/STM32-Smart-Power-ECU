# Verification plan

The verification strategy combines normal-operation tests, boundary tests, fault injection and recovery tests.

## Test categories

- Startup and sensor validation
- Normal operating range
- Warning thresholds
- Qualified FAULT thresholds
- Immediate SHUTDOWN thresholds
- Fault latching and reset
- UART telemetry format
- Relay fail-safe behavior
- Sensor plausibility failures

## Execution

1. Build the firmware in STM32CubeIDE.
2. Load the HEX/ELF into the Proteus STM32 model.
3. Apply the analog inputs from `Tests/test_matrix.csv`.
4. Wait for startup validation.
5. Verify LED state, relay state and UART frame.
6. Record PASS/FAIL and any observed transition time.

## Additional boundary tests

For every threshold, test:
- threshold - epsilon
- threshold
- threshold + epsilon

Example for overtemperature fault at 80 C:
- 79.5 C
- 80.0 C
- 80.5 C

## Fault qualification

The safety task runs every 20 ms and `FAULT_QUALIFICATION_COUNT` is 5, giving approximately 100 ms qualification for non-critical faults.

## Recovery

A latched fault must not reset merely because the signal falls just below the fault threshold. Recovery requires the stricter recovery thresholds defined in `app_config.h`, providing hysteresis.
