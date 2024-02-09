# Drone Flight Controller Outline

## Module Control Flow

Need "mode" state variable, one byte should be enough.

### Input Module

- Get pilot control signals from `receiver.getControls()`

### Sensor Module

- Get sensor data
- Estimate craft state - Difficult, requires Kalman filter

### Flight Mode Module

- Take control signals and calculate target craft state
- Calculate state error based on estimated state - target state
- Send state error into appropriate mode-based PIDs - Medium
    - Error for integral runaway?
- Output process control vector

### Motor Control Module

- Take output process control vector and perform motor control matrix multiplication
- Filter signals to remove high frequency noise
- Send signals into motor ESCs


## Error Codes

| Noise Code | Digit Code | Error message |
| ---------: | ---------: | :------------ |
|    l l l l |          0 | Vector math error - lengths must match |
|    l l l h |          1 | Vector index out of bounds             |
|    l l h l |          2 |               |
|    l l h h |          3 |               |
|    l h l l |          4 |               |
|    l h l h |          5 |               |
|    l h h l |          6 |               |
|    l h h h |          7 |               |
|    h l l l |          8 |               |
|    h l l h |          9 |               |
|    h l h l |         10 |               |
|    h l h h |         11 |               |
|    h h l l |         12 |               |
|    h h l h |         13 |               |
|    h h h l |         14 |               |
|    h h h h |         15 | Unspecified runtime error              |


# To-Do

- Write unit tests for libraries and functions
- ~~Write unit test framework and create an Arduino test suite sketch~~


# Notes/Reminders
- Consider revisting receivers.h and changing it to a zero-overhead abstraction
- ~~Was working on Vector16, which will be used for the matrix equation in the motor control module. The PIDs will output one Vector16.~~
- ~~Was working on unit tests for Vector16, added in Serial message for Buzzer errors.~~
