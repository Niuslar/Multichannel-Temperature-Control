/**
 *@file BME280.c
 *@brief This file contains the BME280 sensor driver code.
 *
 */

/*
 * BME280.c
 *
 *  Created on: 24 Jul 2022
 *      Author: salavat.magazov
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include "gpio.h"
#include "spi.h"

    const uint8_t ctrl_meas = 0b01110100;
    const uint8_t ctrl_hum = 0b01110010;
    const uint8_t config = 0b01110101;
    const uint8_t status = 0b11110011;
    const uint8_t hum_msb = 0b11111101;
    const uint8_t temp_msb = 0b11111010;
    const uint8_t dig_H1_addr = 0b10100001;
    const uint8_t dig_H2_addr = 0b11100001;
    const uint8_t dig_H3_addr = 0b11100011;
    const uint8_t dig_H4_addr = 0b11100100;
    const uint8_t dig_H5_addr = 0b11100101;
    const uint8_t dig_H6_addr = 0b11100111;
    const uint8_t dig_T1_addr = 0b10001000;
    const uint8_t dig_T2_addr = 0b10001010;
    const uint8_t dig_T3_addr = 0b10001100;

    HAL_StatusTypeDef ret;
    uint8_t hum_val[3];   // humidity raw value
    uint8_t temp_val[3];  // temperature raw value
    typedef int32_t BME280_S32_t;
    typedef uint32_t BME280_U32_t;

    BME280_S32_t t_fine, adc_T, adc_H;
    uint8_t spi_buff[5];              // SPI buffer not for measurements
    uint8_t hum_osrs = 0b00000001;    // enable humidity with x1 oversampling
    uint8_t meas_val = 0b00100001;    // set forced mode and turn off press
    uint8_t filter_off = 0b00000000;  // turn off filter
    BME280_U32_t humidity;            // final humidity value
    uint8_t digT1[2];
    int8_t digT2[2], digT3[2], digH2[2];

    unsigned short dig_T1;
    short dig_T2, dig_T3, dig_H2, dig_H4, dig_H5, dig_H4_ls[2], dig_H5_ls[2];
    unsigned char dig_H1, dig_H3, digH6;
    char dig_H6;
    memset(spi_buff, 0, sizeof(spi_buff));
    // select mode 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

    // write settings
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&ctrl_hum, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&hum_osrs, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&ctrl_meas, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&meas_val, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&config, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&filter_off, 1, 100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    spi_buff[0] = 1;

    // check status
    while (spi_buff[0] & 0b00001000)
    {  // bit masking need bit 3

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_SPI_Transmit(&hspi2, (uint8_t *)&status, 1, 100);
        HAL_SPI_Receive(&hspi2, (uint8_t *)spi_buff, 1, 100);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    }

    // read hum data
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&hum_msb, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_val, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_val, 2, 100);
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    adc_H = (hum_val[1] >> 8) | (hum_val[0]);

    // read temp data
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&temp_msb, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_val, 1, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_val, 1, 100);
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    // correct temp data
    adc_T = (temp_val[2] >> 24) | (temp_val[1] >> 21) | temp_val[0];

    // read data
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T1_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT1, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT1, 2, 100);
    }

    dig_T1 = (digT1[1] << 8) | digT1[0];

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T2_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT2, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT2, 2, 100);
    }

    dig_T2 = (int)(digT2[1] << 8) | (int)digT2[0];

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T3_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT3, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT3, 2, 100);
    }

    dig_T3 = (int)((digT3[1] << 8) | digT3[0]);

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H1_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H1, 1, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H1, 1, 100);
    }

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H2_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digH2, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digH2, 2, 100);
    }

    dig_H2 = (int)((digH2[1] << 8) | digH2[0]);

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H3_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H3, 1, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H3, 1, 100);
    }

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H4_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H4_ls, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H4_ls, 2, 100);
    }

    dig_H4_ls[1] = dig_H4_ls[1] & 0b0000000000001111;
    dig_H4 = (int)(dig_H4_ls[0] | dig_H4_ls[1]);

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H5_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H5_ls, 2, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H5_ls, 2, 100);
    }

    dig_H5_ls[0] = (dig_H5_ls[0] & 0b1111000000000000) >> 12;
    dig_H5_ls[1] = dig_H5_ls[1] << 8;
    dig_H5 = (int)(dig_H5_ls[1] | dig_H5_ls[0]);

    HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H6_addr, 1, 100);
    ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&digH6, 1, 100);
    while (ret != HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&digH6, 1, 100);
    }

    dig_H6 = (char)digH6;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    // perform calculations
    BME280_S32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((BME280_S32_t)dig_T1 << 1))) *
            ((BME280_S32_t)dig_T2)) >>
           11;

    var2 = (((((adc_T >> 4) - ((BME280_S32_t)dig_T1)) *
              ((adc_T >> 4) - ((BME280_S32_t)dig_T1))) >>
             12) *
            ((BME280_S32_t)dig_T3)) >>
           14;
    t_fine = var1 + var2;

    BME280_S32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((BME280_S32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)dig_H4) << 20) -
                    (((BME280_S32_t)dig_H5) * v_x1_u32r)) +
                   ((BME280_S32_t)16384)) >>
                  15) *
                 (((((((v_x1_u32r * ((BME280_S32_t)dig_H6)) >> 10) *
                      (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11) +
                       ((BME280_S32_t)32768))) >>
                     10) +
                    ((BME280_S32_t)2097152)) *
                       ((BME280_S32_t)dig_H2) +
                   8192) >>
                  14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((BME280_S32_t)dig_H1)) >>
                              4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    humidity = (BME280_U32_t)(v_x1_u32r >> 12);

#ifdef __cplusplus
}
#endif
