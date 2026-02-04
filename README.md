# Apollon FC
## The hackable open-source drone flight controller that runs on an Arduino

The Apollon Flight Controller (Apollon FC) is an open-source, highly configurable, easily hackable flight controller designed to run entirely on a standard Arduino Uno R3 (ATMEGA328P) for controlling quadcopter RC drones.

Much of the configuration for Apollon FC is done through preprocessor directives to keep the on-board code size small, and lightweight custom libraries are often used in place of prebuilt solutions. Because of this, hardware-agnostic functions can be used to easily switch between sensor boards, radio controllers/receivers, and peripherals. Pin configuration is also handled through preprocessor macro definitions to save on memory while still allowing easy configuration for other boards.

Apollon FC is licensed under the GNU Affero General Public License 3.0, an open-source copyleft license. As such, you are free to use this work in almost any use, with only a few stipulations such as keeping the original license on any modifications. While Apollon FC is currently developed primarily as a personal research and engineering effort, the source is publicly available for reference, learning, and experimentation.

## Design Goals

- High configuration capabilities through the use of macro configuration files rather than runtime branching
- Run entirely on an ATMEGA328P on an Arduino Uno R3 (with potential support for other boards in the future) with no external coprocessors
- Minimize dynamic memory usage and runtime overhead with absolutely no heap usage
- Prioritize deterministic behavior in the hot path
- Maintain hardware flexibility through abstraction with minimal overhead

## Future Work

- Support for additional development boards (Arduino Mega, ESP32, etc.)
- Full AHRS Madgwick filter with IMU and magnetometer fusion
- Possible support for Kalman filtering on higher-end boards

### About the developer
My name is Sabrina Atchley. I have a bachelor's degree in Computer Science and Mathematics from the University of Southern Mississippi (Dec. 2024). I have experience in embedded systems, low-level optimizations, and full-stack web development, and was previously employed at the [National Center for Spectator Sports Safety and Security](https://ncs4.usm.edu) (NCS<sup>4</sup>) where I led development of a custom WordPress site with a tailored theme and plugins.

Apollon FC is a long-running personal engineering project focused on understanding flight controller architecture, real-time software constraints, and performance critical embedded systems development. This project emphasizes extreme configurability while making necessary tradeoffs in accuracy, memory usage, runtime performance, and hardware limitations.

Currently, Apollon FC is in active flight testing. The originally planned Kalman filter module was swapped for an IMU-only Madgwick filter to save memory, minimize runtime cost in the hot path, and accelerate time to first flight. Development has been proceeding for a few years now, and the project is now in the fine-tuning stage.
