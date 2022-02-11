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

## NUCLEO-64 Notes
Nucleo-64 boards have different models with slight differences in their hardware configuration.
The default configuration of some models can interfere with the proper functioning of the firmware. Because of this, some modifications to the configuration might be needed when running the firmware on a Nucleo-64 board instead of a custom one. 

To change the configuration, some solder bridges (SB) and resistors will need to be added/removed.

The location of the SB and resistors that need to be changed for each point below will be marked in the [Hardware Layout](#hardware-layout) with the colour mentioned in brackets. 

### Green LED Pin (Green)
The green user LED that comes with the board is connected to pin PA5 of the MCU. 
To prevent this from interfering with the ADC signal, use the following configuration:
* SB21 OFF

### Pins PC14 and PC15 (Red)
Pins PC14 and PC15 can be used as part of the Low Speed Cock (LSE) or as GPIOs. 
To set the pins as GPIO, the following configuration is needed: 
* SB48 and SB49 ON
* R34 and R36 removed

### HSE Clock Source  (Blue)
Depending on the board model, the HSE might be off or set to use the MCO from the ST-LINK MCU. 
To use the MCO from the ST-LINK, the following configuration is needed: 
* SB55 OFF and SB54 ON
* SB16 and SB50 ON
* R35 and R37 removed

**Note:* If HSE is set to use X3 (Not included), change the RCC in the Device Configuration Tool from HSE: BYPASS Clock Source to HSE: Crystal/Ceramic Resonator. 

### Hardware Layout

<p align="center"><img src="Gallery/HL_top_view.png" width="500" height="500">
<p align="center"><b>Fig. 1 </b>  Hardware Layout Top View. ( Source: 
<a href="https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf">Nucleo-64 User Manual</a> )</p>

<p align="center"><img src="Gallery/HL_bottom_view.png" width="500" height="500">
<p align="center"><b>Fig. 2 </b> Hardware Layout Bottom View. ( Source: 
<a href="https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf">Nucleo-64 User Manual</a> )</p>



## Credits
* [Salavat Magazov](https://github.com/Vavat)
