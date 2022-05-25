#!/usr/bin/env python3

from time import sleep
import pytest
import serial
import sys
import os

#Setup serial comm
ser = serial.Serial(
        port='/dev/ttyAMA0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0.1) 


COMMAND_MALFORMAT = b'Command does not match format.\n'
COMMAND_OK = b'OK.\n'
COMMAND_NOT_RECOGNISED = b'Command not recognised.\n'

#Send command to MCU and return last line read via UART
#Function also prints each line received
def parse_command(command):
    print("Testing: ",command.decode('ascii'),end='')
    ser.write(command)
    answer = bytearray()
    answer_byte = ser.read()
    while(isinstance(answer_byte, bytes) and answer_byte != b''):
        answer.extend(answer_byte)
        answer_byte = ser.read()
    answer_line = bytearray()
    char_counter = 0
    for char in answer:
        char_counter = char_counter + 1
        answer_line.append(char)
        if chr(char) == '\n':
            print(answer_line.decode('ascii'),end='')
            # Clear line unless it is the last line
            if(char_counter < len(answer)):
                answer_line.clear()
    return answer_line

# String commands to be tested
@pytest.mark.parametrize("string_command,string_result", [
    (b'>?temperature()\n',COMMAND_OK),
    (b'>temperature(25)\n',COMMAND_OK),
    (b'command_00\n',COMMAND_MALFORMAT),
    (b'>temperature(string)\n', COMMAND_OK),
    (b'temperature(25)\n', COMMAND_MALFORMAT),
    (b'>temperature(1,2,3,4,5,6,7,8,9,0)\n', COMMAND_OK),
    (b'>temperature(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0)\n', COMMAND_MALFORMAT),
    (b'>temperature(1,,,2,3,4,5,6,7,8,9,0)\n', COMMAND_MALFORMAT),
    (b'>temperature(1, 2, 3, 4, 5, 6, 7, 8, 9, 0)\n', COMMAND_OK),
    (b'>temperature25)\n', COMMAND_MALFORMAT),
    (b'>temperature(25\n', COMMAND_MALFORMAT),
    ])
# Each function with the name test in it will be tested
def test_string_parser(string_command, string_result):
    assert parse_command(string_command) == string_result 


#JSON Commands to be tested
@pytest.mark.parametrize("json_command,json_result",[
    (b'{"?temperature":0}\n',COMMAND_OK),
    (b'{"temperature":44.3}\n',COMMAND_OK),
    (b'{"temperature":[22,8]}\n',COMMAND_OK),
    (b'{"temperature":[1,2,3,4,5,6,7,8,9,0]}\n',COMMAND_OK),
    (b'{"temperature":[1,2,3,4,5,6,7,8,9,0,1]}\n',COMMAND_MALFORMAT),
    (b'{"command5":[33 ,44 ,55 ,66.5 ,77.5 ]}\n',COMMAND_NOT_RECOGNISED),
    (b'{command6:45}\n',COMMAND_MALFORMAT),
    (b'{"command7":[55,77,33,55,33,22,34,4,3,23,2]}\n',COMMAND_MALFORMAT),
    (b'{"command8":\n',COMMAND_MALFORMAT),
    (b'{"command10":{"command11":[10,34]}}\n',COMMAND_MALFORMAT),
    (b'{"command12":"33,22,1134"}\n',COMMAND_NOT_RECOGNISED),
    (b'{sdfsdf"command13":"33,22,1134"}\n',COMMAND_MALFORMAT)
    ])
def test_json_parser(json_command, json_result):
    assert parse_command(json_command) == json_result


