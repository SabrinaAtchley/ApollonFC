# Apollon FC
## The hackable open-source drone flight controller that runs on an Arduino

The Apollon Flight Controller (Apollon FC) is an open-source, highly configurable, easily hackable flight controller which is designed to run entirely on a standard Arduino Uno R3 (ATMEGA328P) for controlling quadcopter RC drones.

Much of the configuration for Apollon FC is done through preprocessor directives to keep the on-board code size small, and lightweight custom libraries are often used in place of prebuilt solutions. Because of this, hardware-agnostic functions can be used to easily switch between sensor boards, radio controllers/receivers, and peripherals. Pin configuration is also handled through preprocessor macro definitions to save on memory while still allowing easy configuration for other boards.

Apollon FC is licensed under the GNU Affero General Public License 3.0, an open-source copyleft license. As such, you are free to use this work in almost any use, with only a few stipulations such as keeping the original license on any modifications. While Apollon FC is open source, this is currently intended as a personal project, and **no plans exist currently to allow other contributors to make additions or modifications to this project**. This may change in the future.


### About the developer
My name is Sabrina Atchley. I attended the University of Southern Mississippi studying Computer Science and Mathematics, and intend to return to finish my degrees by 2025. I have been programming for many years at a hobbyist level, and previously was employed at the [National Center for Spectator Sports Safety and Security](https://ncs4.usm.edu) (NCS<sup>4</sup>) as their web developer in charge of building a new website using WordPress with custom plugins and a custom theme.

Currently, Apollon FC is still within a nascent developmental stage. This project represents the first time I have used Arduino or any microcontroller board, the first time I've used C++ macros in any capacity, the first time I have made my own basic test framework, the first time I have made and used unit tests, the first time I have used PIDs, sensor fusion, radio communications, serial communications, and many, many more firsts. This project is being created by someone who has never flown a drone, and certainly has no idea how a flight controller works other than what little I have cobbled together online, or thought up myself. So, in short: I've learned a lot from this.
