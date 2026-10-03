# Smart Patient Assistant

> **Project started on October 1, 2026**  
> **Current release: V1 — Scheduled Stepper Prototype**

Smart Patient Assistant is a hardware project I am building to make patient care easier and more reliable through automation.

The long-term goal is a patient-assistance system that can handle scheduled physical actions and, later, connect those actions with a larger smart-assistant system.

Right now, the project is in its **V1 hardware stage**. V1 focuses on one important part of the system: **accurate time-based motor control using an ESP32 and DS3231 RTC**.

---

## What is this project?

The Smart Patient Assistant is being developed as a modular hardware system.

The current V1 prototype can:

- Keep time using a **DS3231 RTC**
- Control **3 stepper motors**
- Store multiple time-based actions in the firmware
- Check the current time every second
- Trigger the correct motor when a scheduled time is reached
- Automatically release the motor after movement

The motor system is the first building block of the larger patient-assistance project.

---

## Why I started this

I wanted to build a practical electronics project where software, embedded systems, and mechanical control work together to solve a real-world problem.

A patient-assistance device needs to be dependable. That means the system should not depend completely on a computer being connected all the time.

Because of that, the project is being designed around:

- An **ESP32** as the main controller
- A dedicated **DS3231 RTC** for keeping time
- Local hardware control
- Persistent schedule storage in a later version
- Remote schedule management through Firebase in a later version

The first step is getting the hardware itself working reliably.

---

# V1: Scheduled Stepper Prototype

The first version in this repository is:

`1_run_stepper_on_time.ino`

V1 is intentionally simple. It is a testable hardware foundation rather than the complete patient assistant.

### How V1 works

```text
              DS3231 RTC
                  |
                  | I2C
                  v
               ESP32
                  |
        +---------+---------+
        |         |         |
        v         v         v
     Motor 1   Motor 2   Motor 3
```

The ESP32 reads the time from the DS3231.

Every second, the firmware checks whether the current hour and minute match one of the configured schedules.

When a schedule matches, the required stepper motor is moved.

---

## Example V1 schedule

The current prototype uses schedules like:

```cpp
Schedule schedules[] = {
  {14, 58, 1, 0, 0},
  {17, 54, 2, 1, 0},
  {21, 15, 0, 0, 3}
};
```

This means:

| Time | Motor 1 | Motor 2 | Motor 3 |
|---|---:|---:|---:|
| 14:58 | 1 | 0 | 0 |
| 17:54 | 2 | 1 | 0 |
| 21:15 | 0 | 0 | 3 |

The current firmware also prevents the same schedule from being triggered repeatedly during the same minute.

---

# Hardware

## Controller

- ESP32

## RTC

- DS3231 RTC module
- I2C SDA → GPIO 21
- I2C SCL → GPIO 22

## Stepper Motor 1

| Signal | GPIO |
|---|---:|
| IN1 | 18 |
| IN2 | 19 |
| IN3 | 23 |
| IN4 | 25 |

## Stepper Motor 2

| Signal | GPIO |
|---|---:|
| IN1 | 26 |
| IN2 | 27 |
| IN3 | 32 |
| IN4 | 33 |

## Stepper Motor 3

| Signal | GPIO |
|---|---:|
| IN1 | 16 |
| IN2 | 17 |
| IN3 | 13 |
| IN4 | 14 |

> GPIO 21 and GPIO 22 are reserved for the DS3231 I2C connection in the current design.

---

# Stepper control

V1 uses an 8-step sequence for the motor coils:

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

Current firmware settings:

- **4096 steps/revolution**
- **3 ms step delay**
- Motor coils are released after movement

The requested motor movement is converted into a number of steps by the firmware.

---

# Tech Stack

### Hardware

- ESP32
- DS3231 RTC
- 3 stepper motors
- Stepper motor driver/interface hardware

### Software

- Arduino IDE
- C/C++
- Arduino ESP32 core
- RTClib
- GitHub for source control

### Planned services

- Firebase Realtime Database
- ESP32 Preferences / NVS for local schedule storage
- Wi-Fi

These planned services are not part of the current V1 implementation yet.

---

# How to run V1

## Requirements

You need:

- An ESP32 board
- DS3231 RTC module
- Three compatible stepper motor/driver setups
- Arduino IDE
- ESP32 board support installed in Arduino IDE
- RTClib installed

## 1. Clone the repository

```sh
git clone https://github.com/abinsaji2008/smart-patient-assistant.git
cd smart-patient-assistant
```

## 2. Open the V1 firmware

Open:

```text
1_run_stepper_on_time.ino
```

## 3. Install the library

In Arduino IDE, install:

```text
RTClib
```

## 4. Select your ESP32

Select the correct ESP32 board and COM port.

## 5. Connect the hardware

Follow the GPIO tables in the **Hardware** section.

## 6. Upload

Upload the sketch to the ESP32.

## 7. Open Serial Monitor

Use:

```text
115200 baud
```

You should see output similar to:

```text
RTC FOUND
HM: HHMM
```

When a schedule matches:

```text
Schedule matched: HH:MM
```

---

# Current project status

### V1 — Working foundation

- [x] ESP32 motor control
- [x] DS3231 RTC communication
- [x] Three stepper motor outputs
- [x] Time-based scheduling
- [x] Motor release after movement
- [x] Same-minute trigger protection

### Next versions

- [ ] Support exactly 6 schedules
- [ ] Save schedules in ESP32 internal NVS memory
- [ ] Load schedules automatically after reboot
- [ ] Connect schedules to Firebase Realtime Database
- [ ] Add Wi-Fi reconnect handling
- [ ] Detect and apply Firebase schedule changes
- [ ] Add patient-assistance functions
- [ ] Add mechanical fault/jam handling
- [ ] Add better safety checks
- [ ] Build and document the complete physical enclosure
- [ ] Add photos and test documentation for the finished hardware

---

# Planned V2 architecture

The planned next stage is:

```text
              Firebase
                 |
          schedule updates
                 |
                 v
              ESP32
           /           \
          v             v
       RAM          NVS Flash
     schedule       backup copy
          \           /
           v         v
             DS3231
                |
                v
        Scheduled motor action
```

The idea is that Firebase will be used to change the schedule, while the ESP32 keeps a local copy.

That means the device can continue following the last saved schedule even when Wi-Fi or Firebase is temporarily unavailable.

The target is **6 schedules maximum**.

---

# Development notes

This project is being developed incrementally rather than as one large final program.

V1 is deliberately focused on proving that:

1. the ESP32 can control the motors,
2. the RTC keeps the schedule accurate,
3. the firmware can detect the correct time, and
4. the physical motor action can be triggered reliably.

The Firebase and persistent-memory parts will be added after the basic hardware behavior is stable.

---

# AI disclosure

AI tools were used during development for **debugging help, code troubleshooting, implementation suggestions, documentation assistance, and explaining technical concepts**.

The project direction, hardware choices, wiring decisions, testing, and physical implementation are part of my development work.

---

# Screenshots and build photos

This is a hardware project, so build photos are an important part of the project documentation.

Photos will be added here as the prototype is assembled and tested.

Suggested documentation:

- Full prototype photo
- ESP32 + DS3231 wiring
- Stepper motor/driver wiring
- Close-up of the mechanical mechanism
- Photo/video of a scheduled motor action

---

# Safety

This is an engineering prototype and **not a certified medical device**.

It should not be used to dispense medication to a patient without appropriate engineering validation, mechanical safeguards, fault detection, dose verification, power-failure handling, manual override, and independent safety testing.

---

# Project timeline

| Date | Milestone |
|---|---|
| **October 1, 2026** | Project started |
| **October 3, 2026** | V1 scheduled stepper prototype documented |
| **Next** | Persistent schedule storage + Firebase integration |
| **Later** | Full patient-assistance system |

---

# Repository

**GitHub:**  
https://github.com/abinsaji2008/smart-patient-assistant

---

## Note

This repository is actively being developed. The current code represents **V1 of the hardware/control portion**, not the finished Smart Patient Assistant.

