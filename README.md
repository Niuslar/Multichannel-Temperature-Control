# Multi-channel temperature control

The goal of this project is to develop a temperature control system for an incubator based on an STM32L053 MCU. 

This particular module is responsible for accurate temperature control in a very large container. The container houses reaction vessels as well as robotic actuators for manipulating the vessels. 
To achieve this, the temperature control system will include the following features.

## Features

* Comms channels (UART and I2C) for logging data and sending commands
* 10 ADC Channels for temperature reading
* 10 Control Channels for heaters/coolers using timers
* 4 ADC Channels for internal telemetry
* PID Control loop to regulate temperature
* LEDs to monitor general application status


## Credits
* [Salavat Magazov](https://github.com/Vavat)
