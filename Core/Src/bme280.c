/*
 * bme280.c
 *
 *  Created on: Jul. 23, 2022
 *      Author: bossm
 */

#include "bme280.h"
#include <string.h>

bme280_u32_t bme280(SPI_HandleTypeDef hspi2){
	//select mode 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	//write settings
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&ctrl_hum, 1, 100);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&hum_osrs, 1, 100);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&ctrl_meas, 1, 100);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&meas_val, 1, 100);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&config, 1, 100);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&filter_off, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	memset(spi_buff, 0, sizeof(spi_buff));
	spi_buff[0]=1;

	//check status
	while(spi_buff[0] & (1<<3)){ //bit masking need bit 3
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_SPI_Transmit(&hspi2, (uint8_t *)&status, 1, 100);
		HAL_SPI_Receive(&hspi2, (uint8_t *)spi_buff, 1, 100);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	}

	//read hum data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&hum_msb, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_raw_val, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_raw_val, 2, 100);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	adc_h = (hum_raw_val[1]>>8)|(hum_raw_val[0]);

	//read temp data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&temp_msb, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_raw_val, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_raw_val, 1, 100);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	//correct temp data
	adc_t = (temp_raw_val[2] >> 24)|(temp_raw_val[1] >> 21)|temp_raw_val[0];

	//read data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_t1_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t1, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t1, 2, 100);
	}

	dig_t1 = (t1[1] << 8) | t1[0];

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_t2_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t2, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t2, 2, 100);
	}

	dig_t2 = (int)(t2[1] << 8) | (int) t2[0];

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_t3_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t3, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)t3, 2, 100);
	}

	dig_t3 = (int) ((t3[1] << 8) | t3[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h1_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_h1, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_h1, 1, 100);
	}

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h2_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)h2, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)h2, 2, 100);
	}

	dig_h2 = (int) ((h2[1] << 8) | h2[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h3_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_h3, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_h3, 1, 100);
	}

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h4_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_h4_ls, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_h4_ls, 2, 100);
	}

	dig_h4_ls[1] = dig_h4_ls[1] & 0xF;
	dig_h4 = (int) (dig_h4_ls[0] | dig_h4_ls[1]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h5_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_h5_ls, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_h5_ls, 2, 100);
	}

	dig_h5_ls[0] = (dig_h5_ls[0] & 0xF000) >> 12;
	dig_h5_ls[1] = dig_h5_ls[1] << 8;
	dig_h5 = (int) (dig_h5_ls[1] | dig_h5_ls[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_h6_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&h6, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&h6, 1, 100);
	}

	dig_h6 = (char) h6;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	return bme280CompensateH(adc_h);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
void bme280CompensateT(bme280_s32_t adc_t){
	bme280_s32_t var1, var2;
	var1 = ((((adc_t>>3) - ((bme280_s32_t)dig_t1<<1))) * ((bme280_s32_t)dig_t2)) >> 11;
	var2 = (((((adc_t>>4) - ((bme280_s32_t)dig_t1)) * ((adc_t>>4) - ((bme280_s32_t)dig_t1))) >> 12) *
	((bme280_s32_t)dig_t3)) >> 14;
	t_fine = var1 + var2;
	//T = (t_fine * 5 + 128) >> 8;
	//return T;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
bme280_u32_t bme280CompensateH(bme280_s32_t adc_h){
	bme280CompensateT(adc_t);
	bme280_s32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((bme280_s32_t)76800));
	v_x1_u32r = (((((adc_h << 14) - (((bme280_s32_t)dig_h4) << 20) - (((bme280_s32_t)dig_h5) * v_x1_u32r)) +
	((bme280_s32_t)16384)) >> 15) * (((((((v_x1_u32r * ((bme280_s32_t)dig_h6)) >> 10) * (((v_x1_u32r *
	((bme280_s32_t)dig_h3)) >> 11) + ((bme280_s32_t)32768))) >> 10) + ((bme280_s32_t)2097152)) *
	((bme280_s32_t)dig_h2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((bme280_s32_t)dig_h1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (bme280_u32_t)(v_x1_u32r>>12);
}
