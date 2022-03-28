#!/usr/bin/env expect
set timeout -1

spawn /tmp/stm32cubeide-installer.sh
expect -ex "--More--"
send "q"
expect -ex "I ACCEPT (y) / I DO NOT ACCEPT (N)"
send "y\r"
#expect -ex "STM32CubeIDE install dir? [/opt/st/stm32cubeide_1.8.0]"
send "\r"
expect -ex "Do you want to install Segger J-Link udev rules?"
send "n\r"
expect "STM32CubeIDE installed successfully"
send_user "auto install STM32 Cube IDE done\r"
exit