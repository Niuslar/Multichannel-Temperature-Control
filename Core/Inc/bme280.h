/*
 * bme280.h
 *
 *  Created on: Jul. 23, 2022
 *      Author: bossm
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"
#define MAX_RAW_HUM_BYTES 3
#define MAX_RAW_TEMP_BYTES 3
#define SPI_BUFF_LEN 5
#define DOUBLE_BYTE_LEN 2

const uint8_t ctrl_meas = 0x74;
const uint8_t ctrl_hum = 0x72;
const uint8_t config = 0x75;
const uint8_t status = 0xF3;
const uint8_t hum_msb = 0xFD;
const uint8_t temp_msb = 0xFA;
const uint8_t dig_h1_addr = 0xA1;
const uint8_t dig_h2_addr = 0xE1;
const uint8_t dig_h3_addr = 0xE3;
const uint8_t dig_h4_addr = 0xE4;
const uint8_t dig_h5_addr = 0xE5;
const uint8_t dig_h6_addr = 0xE7;
const uint8_t dig_t1_addr = 0x88;
const uint8_t dig_t2_addr = 0x8A;
const uint8_t dig_t3_addr = 0x8C;

typedef int32_t bme280_s32_t;
typedef uint32_t bme280_u32_t;
bme280_s32_t t_fine, adc_t, adc_h;
void bme280CompensateT(bme280_s32_t adc_t);
bme280_u32_t bme280CompensateH(bme280_s32_t adc_h);

uint8_t t1[DOUBLE_BYTE_LEN];
uint16_t dig_t1;
int8_t t2[DOUBLE_BYTE_LEN], t3[DOUBLE_BYTE_LEN], h2[DOUBLE_BYTE_LEN];
int16_t dig_t2, dig_t3, dig_h2, dig_h4, dig_h5, dig_h4_ls[DOUBLE_BYTE_LEN], dig_h5_ls[DOUBLE_BYTE_LEN];
unsigned char dig_h1, dig_h3, h6;
char dig_h6;

uint8_t spi_buff[SPI_BUFF_LEN];
uint8_t hum_osrs = 0x1; //enable humidity with x1 oversampling
uint8_t meas_val = 0x21; //set forced mode and turn off pressure
uint8_t filter_off = 0x0;
uint8_t hum_raw_val[MAX_RAW_HUM_BYTES];
uint8_t temp_raw_val[MAX_RAW_TEMP_BYTES];
HAL_StatusTypeDef ret;

#endif /* INC_BME280_H_ */
