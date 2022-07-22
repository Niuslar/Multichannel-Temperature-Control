/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void cpp_main();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /**
     * @notes
     *
     * 1. DMA is not setup.
     * 2. ADC conversion sequence is not setup.
     * 3. C++ integration is not setup.
     * 4. Test if DMA bug is present for this MCU.
     * 5. Check timing in all classes. Main clock is now much higher. APB1 and
     * APB2 are on different clocks.
     * 6. Run on real hardware...
     * 7. Full regression test as much as possible. If don't know what
     * regression test is, read up on it.
     */
	HAL_StatusTypeDef ret;
	uint8_t hum_val[3]; //humidity raw value
	uint8_t temp_val[3]; //temperature raw value
	typedef int32_t BME280_S32_t;
	typedef uint32_t BME280_U32_t;
	BME280_S32_t t_fine, adc_T, adc_H;
	uint8_t spi_buff[5]; //SPI buffer not for measurements
	uint8_t hum_osrs = 0b00000001; //enable humidity with x1 oversampling
	uint8_t meas_val = 0b00100001; //set forced mode and turn off press
	uint8_t filter_off = 0b00000000; //turn off filter
	BME280_U32_t humidity; //final humidity value
	uint8_t digT1[2];
	int8_t digT2[2], digT3[2], digH2[2];
	unsigned short dig_T1;
	short dig_T2, dig_T3, dig_H2, dig_H4, dig_H5, dig_H4_ls[2], dig_H5_ls[2];
	unsigned char dig_H1, dig_H3, digH6;
	char dig_H6;
	memset(spi_buff, 0, sizeof(spi_buff));
    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_DMA_Init();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    MX_TIM10_Init();
    MX_TIM11_Init();
    MX_SPI2_Init();
    MX_TIM4_Init();
    /* USER CODE BEGIN 2 */
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

    spi_buff[0]=1;

    //check status
    while(spi_buff[0] & 0b00001000){ //bit masking need bit 3
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
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_val, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)hum_val, 2, 100);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	adc_H = (hum_val[1]>>8)|(hum_val[0]);

    //read temp data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&temp_msb, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_val, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)temp_val, 1, 100);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	//correct temp data
	adc_T = (temp_val[2] >> 24)|(temp_val[1] >> 21)|temp_val[0];

	//read data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T1_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT1, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT1, 2, 100);
	}

	dig_T1 = (digT1[1] << 8) | digT1[0];

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T2_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT2, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT2, 2, 100);
	}

	dig_T2 = (int)(digT2[1] << 8) | (int) digT2[0];

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_T3_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT3, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digT3, 2, 100);
	}

	dig_T3 = (int) ((digT3[1] << 8) | digT3[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H1_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H1, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H1, 1, 100);
	}

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H2_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digH2, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)digH2, 2, 100);
	}

	dig_H2 = (int) ((digH2[1] << 8) | digH2[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H3_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H3, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&dig_H3, 1, 100);
	}

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H4_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H4_ls, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H4_ls, 2, 100);
	}

	dig_H4_ls[1] = dig_H4_ls[1] & 0b0000000000001111;
	dig_H4 = (int) (dig_H4_ls[0] | dig_H4_ls[1]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H5_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H5_ls, 2, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (uint8_t *)dig_H5_ls, 2, 100);
	}

	dig_H5_ls[0] = (dig_H5_ls[0] & 0b1111000000000000) >> 12;
	dig_H5_ls[1] = dig_H5_ls[1] << 8;
	dig_H5 = (int) (dig_H5_ls[1] | dig_H5_ls[0]);

	HAL_SPI_Transmit(&hspi2, (uint8_t *)&dig_H6_addr, 1, 100);
	ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&digH6, 1, 100);
	while(ret != HAL_OK){
		ret = HAL_SPI_Receive(&hspi2, (unsigned char *)&digH6, 1, 100);
	}

	dig_H6 = (char) digH6;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	//perform calculations
	BME280_S32_t var1, var2;
	var1 = ((((adc_T>>3) - ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((BME280_S32_t)dig_T1)) * ((adc_T>>4) - ((BME280_S32_t)dig_T1))) >> 12) *
	((BME280_S32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;

	BME280_S32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((BME280_S32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)dig_H4) << 20) - (((BME280_S32_t)dig_H5) * v_x1_u32r)) +
	((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r * ((BME280_S32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11)
	+ ((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((BME280_S32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

	humidity = (BME280_U32_t) (v_x1_u32r >> 12);
    /* From here the application moves to the code found in cpp_link.cpp */
    cpp_main();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 100;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
