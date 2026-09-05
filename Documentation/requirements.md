# Requirements and traceability

## Safety requirements

| ID | Requirement | Implementation | Verification |
|---|---|---|---|
| REQ-SAF-001 | If battery voltage exceeds 16 V, the ECU shall disconnect the load. | `Safety_Update()` | TC04 |
| REQ-SAF-002 | If battery voltage falls below 9 V, the ECU shall disconnect the load. | `Safety_Update()` | dedicated undervoltage test |
| REQ-SAF-003 | If current exceeds 5.5 A, the ECU shall disconnect the load. | `Safety_Update()` | TC07 |
| REQ-SAF-004 | If temperature exceeds 90 C, the ECU shall disconnect the load. | `Safety_Update()` | TC10 |
| REQ-SAF-005 | A non-critical fault shall be qualified for at least 100 ms before latching. | `FAULT_QUALIFICATION_COUNT` | TC03/TC06/TC09 timing test |
| REQ-SAF-006 | The relay shall remain OFF until valid sensor data is available. | `Sensors_IsReady()` | startup test |
| REQ-SAF-007 | A latched fault shall only be reset when recovery conditions are satisfied. | `Safety_ResetLatchedFault()` | recovery test |

## Diagnostic fault codes

| Code | Meaning |
|---:|---|
| 0 | No fault |
| 1 | Undervoltage |
| 2 | Overvoltage |
| 3 | Overcurrent |
| 4 | Overtemperature |
| 5 | Sensor plausibility fault |

## Traceability concept

```text
System Requirement
      -> Software Requirement
          -> C Function / Module
              -> Test Case
                  -> Test Result
```

This structure is intentionally simple so the project can later be extended to DOORS-style requirements management or a formal safety case.
