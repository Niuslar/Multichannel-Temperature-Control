#!/usr/bin/env python3

from time import sleep
import RPi.GPIO as GPIO
import serial
import sys
import os

# RPi GPIO Pins used for flashing
LED = 7
BOOT0 = 3
BOOT1 = 5
RESET = 11

# Bootloader commands
uart_select_command = b'\x7F'

write_memory = bytearray()
write_memory.append(0x31)
write_memory.append(0xCE)

erase_memory = bytearray()
erase_memory.append(0x44)
erase_memory.append(0xBB)

global_erase = bytes.fromhex('ffff')


# Flash memory address
flash_addr = bytearray()
flash_addr.append(0x08)
flash_addr.append(0x00)
flash_addr.append(0x00)
flash_addr.append(0x00)

start_address = 0x08000000

ACK_BYTE = 0x79
NACK_BYTE = 0x1F

#Setup serial comm
ser = serial.Serial(
        port='/dev/ttyAMA0',
        baudrate=115200,
        parity=serial.PARITY_EVEN,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=15) 

def setupPeripherals():      
    # Setup RPi GPIOs
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(LED, GPIO.OUT)
    GPIO.setup(BOOT0,GPIO.OUT)
    GPIO.setup(BOOT1,GPIO.OUT)
    GPIO.setup(RESET,GPIO.OUT)

def beginFlash():
    # Setup STM32 Bootloader 
    print('Activating STM32F411 Bootloader')
    GPIO.output(BOOT0,GPIO.HIGH)
    GPIO.output(BOOT1,GPIO.LOW)

    # Reset STM32
    GPIO.output(RESET,GPIO.LOW)
    sleep(0.1)
    GPIO.output(RESET,GPIO.HIGH)
    sleep(1)


# Send commands 
def startCommands():
    b_success = False
    ser.write(uart_select_command)
    response_byte = ser.read()
    if int(response_byte.hex(),16) == ACK_BYTE: 
        print("UART Recognised")
        print("Erasing flash memory")
        b_success = True
    return b_success


def eraseMemory():
    b_success = False
    ser.write(erase_memory)
    response_byte = ser.read()
    if int(response_byte.hex(),16) == ACK_BYTE: 
        ser.write(global_erase)
        ser.write(b'\x00') # Checksum for global erase command
        response_byte = ser.read()
        if int(response_byte.hex(),16) == ACK_BYTE:
            print("Flash memory successfuly erased")
            b_success = True
    if b_success == False:
        print("Error erasing flash memory")
    return b_success


def flashSTM32(file_name, file_size):
    b_success = False
    block_counter = 0
    print("Downloading ", file_size, " bytes")
    # Each block can be at most 256 bytes. 
    # Check how many blocks I need to write 
    block_size = 256
    blocks = int(file_size/block_size)

    write_address = start_address

    # Open file 
    f = open(file_name,'rb') 
    for i in range(blocks):
        if writeMemory(f, block_size) == True:
            block_counter = block_counter + 1
        write_address = write_address + 256
        updateAddressByteArray(write_address)
    
    # Send data that didn't fit in a block
    rest_bytes = file_size - (blocks*block_size)
    if rest_bytes < 0 or rest_bytes > 255:
        b_success = False
    else:
        if writeMemory(f, rest_bytes) == True:
            b_success = True
            block_counter = block_counter + 1

    f.close()
    return b_success


def writeMemory(file_id, block_size):
    b_success = False
    # Send write memory command
    ser.write(write_memory)
    response_byte = ser.read()

    # If the command is recognised, start writing bytes
    if int(response_byte.hex(),16) == ACK_BYTE:
        # Read block of data from .bin file 
        buffer = file_id.read(block_size)
        # Calculate data checksum
        data_checksum = calcChecksum(block_size, buffer)

        # Calculate address checksum
        addr_checksum = flash_addr[0] ^ flash_addr[1] ^ flash_addr[2] ^ flash_addr[3]
        addr_checksum_ba = bytearray()
        addr_checksum_ba.append(addr_checksum)

        # Send address and "checksum"
        ser.write(flash_addr)
        ser.write(addr_checksum_ba)
        if int(response_byte.hex(),16) == ACK_BYTE:
            #Send number of bytes to write N-1
            data_payload = bytearray()
            data_payload.append(block_size - 1)
            for byte in buffer:
                data_payload.append(byte)
            data_payload.append(data_checksum)
            ser.write(data_payload)
            response_byte = ser.read()
            if int(response_byte.hex(),16) == ACK_BYTE:
                print('.',end='', flush=True)
                GPIO.output(LED, GPIO.HIGH)
                sleep(0.02)
                GPIO.output(LED, GPIO.LOW)
                sleep(0.03)
                b_success = True
    return b_success
        

def updateAddressByteArray(new_address):
    flash_addr[0] = (new_address >> 24) & 0xFF   # This gets the MSB
    flash_addr[1] = (new_address >> 16) & 0xFF
    flash_addr[2] = (new_address >> 8) & 0xFF
    flash_addr[3] = (new_address) & 0xFF         # LSB

def calcChecksum(buf_size, buffer):
    n_ba = bytearray()
    N = buf_size - 1
    n_ba.append(N)
    checksum =  n_ba[0] ^ buffer[0]
    for index in range(buf_size - 1):
        checksum = checksum ^ buffer[index + 1]
        
    return checksum
    

def endFlash():
    # Restart board
    print('Reseting STM32F')
    GPIO.output(BOOT0, GPIO.LOW)
    GPIO.output(RESET, GPIO.LOW)
    sleep(0.1)
    GPIO.output(RESET,GPIO.HIGH)

    # Finishing routine
    GPIO.cleanup()
    ser.close()

max_flash_size = 512*1024 #512kB

def main():
    if len(sys.argv) != 2:
        print("Use: ./stm32_flash_loader.py path/to/file.bin")
        return
    filename = sys.argv[-1]
    file_size = os.path.getsize(str(filename))
    if file_size > max_flash_size:
        print("Error, .bin file too big")
        return
    setupPeripherals()
    beginFlash()
    if startCommands() == True:
        if eraseMemory() == True:
            if flashSTM32(str(filename), file_size) == True:
                print("\nDownload of ", file_size, " successful")
            else: 
                print("Error writing into flash")
        else:
            print("Error erasing memory")
    else:
        print("Error sending write memory command")
    endFlash()


if __name__ == "__main__":
    main()