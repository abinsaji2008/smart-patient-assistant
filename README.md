# Smart Patient Assistant

> **Project started on October 1, 2026**

An ESP32-based smart patient-assistance project focused on scheduled dispensing/control, with an RTC-driven motor system as the current hardware stage.

## Current Status

The repository currently contains the first working motor-control prototype:

- ESP32
- DS3231 RTC
- 3 stepper motors
- Scheduled motor activation based on RTC time
- Up to 6 planned schedules for the next scheduling stage

The current sketch stores schedules directly in the code. Firebase synchronization and persistent internal-memory storage are planned next.

## Current Firmware

The current prototype is:

`1_run_stepper_on_time.ino`

It:

1. Initializes the DS3231 RTC over I2C.
2. Initializes three stepper-motor interfaces.
3. Reads the current RTC time once per second.
4. Checks the configured schedule.
5. Runs the corresponding motor when a schedule matches.
6. Releases the motor coils after movement.

## Hardware

### RTC

| Function | GPIO |
|---|---:|
| I2C SDA | 21 |
| I2C SCL | 22 |

### Motor 1

| Signal | GPIO |
|---|---:|
| IN1 | 18 |
| IN2 | 19 |
| IN3 | 23 |
| IN4 | 25 |

### Motor 2

| Signal | GPIO |
|---|---:|
| IN1 | 26 |
| IN2 | 27 |
| IN3 | 32 |
| IN4 | 33 |

### Motor 3

| Signal | GPIO |
|---|---:|
| IN1 | 16 |
| IN2 | 17 |
| IN3 | 13 |
| IN4 | 14 |

> GPIO 21 and 22 are reserved for the DS3231 I2C bus in the current design.

## Stepper Configuration

The firmware currently uses an 8-step sequence:

```text
1,0,0,0
1,1,0,0
0,1,0,0
0,1,1,0
0,0,1,0
0,0,1,1
0,0,0,1
1,0,0,1
```

The current firmware assumes:

- `4096` steps per revolution
- `3 ms` step delay
- Motor movement is calculated from the requested number of parts/turns

## Current Schedule Format

The current firmware uses:

```cpp
struct Schedule {
  int hour;
  int minute;
  int motor1Turns;
  int motor2Turns;
  int motor3Turns;
};
```

Example:

```cpp
Schedule schedules[] = {
  {14, 58, 1, 0, 0},
  {17, 54, 2, 1, 0},
  {21, 15, 0, 0, 3}
};
```

This means:

- 14:58 → Motor 1
- 17:54 → Motor 1 + Motor 2
- 21:15 → Motor 3

The current checker prevents the same schedule from executing repeatedly during the same minute.

## Planned Architecture

The next firmware stage is intended to move from hard-coded schedules to Firebase-controlled schedules with local persistence:

```text
Firebase Realtime Database
          ↓
       ESP32
          ↓
   Internal NVS memory
          ↓
      RAM schedule
          ↓
       DS3231
          ↓
    Motor controller
```

The target is a maximum of **6 schedules**.

This design allows the device to continue using the last saved schedule when Wi-Fi or Firebase is temporarily unavailable.

## Planned Schedule Storage

The planned schedule structure is:

```cpp
#define MAX_SCHEDULES 6

Schedule schedules[MAX_SCHEDULES];
int NUMBER_OF_SCHEDULES = 0;
```

Firebase credentials and other private configuration should be kept outside the main source file, for example in:

```text
secret.h
```

Do not commit real Wi-Fi passwords, API keys, or other secrets to the public repository.

## Software

Recommended environment:

- Arduino IDE
- ESP32 board support package
- RTClib

Libraries used by the current prototype:

```cpp
#include <Wire.h>
#include "RTClib.h"
```

The Firebase/NVS stage will additionally require the appropriate Firebase and preferences/JSON libraries for the selected implementation.

## Getting Started

1. Install Arduino IDE.
2. Install ESP32 board support.
3. Install the `RTClib` library.
4. Connect the DS3231 to GPIO 21/22.
5. Connect the three stepper drivers/motor interfaces according to the pin map above.
6. Open `1_run_stepper_on_time.ino`.
7. Select the correct ESP32 board and COM port.
8. Upload the sketch.
9. Open Serial Monitor at `115200` baud.

Expected startup messages include:

```text
RTC FOUND
HM: HHMM
```

When a schedule matches:

```text
Schedule matched: HH:MM
```

## Safety

This repository is an engineering prototype. It is not a certified medical device.

A real medication-dispensing implementation should include appropriate mechanical safeguards, fault detection, dose verification, power-failure handling, jam detection, manual override, and independent safety validation before being used for patient care.

## Roadmap

- [x] DS3231 RTC integration
- [x] Three-stepper control
- [x] Time-based scheduling
- [ ] Limit schedule storage to 6 entries
- [ ] Save schedules in ESP32 NVS
- [ ] Load schedules after reboot
- [ ] Firebase Realtime Database synchronization
- [ ] `secret.h` configuration
- [ ] Wi-Fi reconnect handling
- [ ] Schedule update detection
- [ ] Fault and motor-jam handling
- [ ] Patient-assistance functions
- [ ] End-to-end system testing

## License

No license has been specified yet.
