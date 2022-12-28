/**
 *@file CBME280.h
 *
 */

/*
 * CBME280.h
 *
 *  Created on: 24 Jul 2022
 *      Author: salavat.magazov
 */

#ifndef CBME280_H_
#define CBME280_H_

#include "CGpioWrapper.h"
#include "gpio.h"
#include "spi.h"

#define T_CALIBRATION_SIZE 3
#define P_CALIBRATION_SIZE 9
#define H_CALIBRATION_SIZE 6
#define RAW_ADC_DATA_SIZE  8
#define MAX_SENSORS        2

class CBME280
{
    enum REGISTER_MAP
    {
        /* Calibration registers. */
        DIG_T1 = 0x88,
        DIG_T2 = 0x8A,
        DIG_T3 = 0x8C,
        DIG_P1 = 0x8E,
        DIG_P2 = 0x90,
        DIG_P3 = 0x92,
        DIG_P4 = 0x94,
        DIG_P5 = 0x96,
        DIG_P6 = 0x98,
        DIG_P7 = 0x9A,
        DIG_P8 = 0x9C,
        DIG_P9 = 0x9E,
        DIG_H1 = 0xA1,
        DIG_H2 = 0xE1,
        DIG_H3 = 0xE3,
        DIG_H4 = 0xE4,
        DIG_H5 = 0xE5,
        /* Sensor data registers. */
        ID = 0xD0,
        RESET = 0xE0,
        CTRL_HUM = 0xF2,
        STATUS = 0xF3,
        CTRL_MEAS = 0xF4,
        CONFIG = 0xF5,
        PRESS_MSB = 0xF7,
        PRESS_LSB = 0xF8,
        PRESS_XLSB = 0xF9,
        TEMP_MSB = 0xFA,
        TEMP_LSB = 0xFB,
        TEMP_XLSB = 0xFC,
        HUM_MSB = 0xFD,
        HUM_LSB = 0xFE
    };

public:
    CBME280();
    virtual ~CBME280();

    bool init(SPI_HandleTypeDef *p_spi,
              GPIO_TypeDef *p_slave_select_port,
              uint16_t slave_select_pin);
    bool run();

    /**
     * @brief Access processed temperature data.
     *
     * @return Temperature in degrees Celsius.
     */
    float getTemperature() const
    {
        return m_temperature;
    };

    /**
     * @brief Access processed pressure data.
     *
     * @return Pressure in Pascals.
     */
    float getPressure() const
    {
        return m_pressure;
    };

    /**
     * @brief Access processed humidity data.
     *
     * @return Humidity in %RH.
     */
    float getHumidity() const
    {
        return m_humidity;
    };

    bool isMeasuring() const
    {
        return m_state == MEASURING;
    }

    static void processIrq();

private:
    void calibrateSensor(uint8_t const *const p_calibration_data);
    void convertRawData();
    bool startMeasurement();
    void calculateT();
    void calculateP();
    void calculateH();
    bool init();

    SPI_HandleTypeDef *mp_spi;
    CGpioWrapper m_slave_select;

    enum STATES
    {
        INIT_PENDING = 0,
        READY,
        MEASURING,
        NEW_DATA
    } m_state;

    static CBME280 *sp_sensors[MAX_SENSORS];
    static uint8_t s_sensor_count;

    float m_temperature_calibration[T_CALIBRATION_SIZE];
    float m_pressure_calibration[P_CALIBRATION_SIZE];
    float m_humidity_calibration[H_CALIBRATION_SIZE];

    uint8_t m_raw_adc_data[RAW_ADC_DATA_SIZE];
    uint32_t m_raw_temperature_data;
    uint32_t m_raw_pressure_data;
    uint32_t m_raw_humidity_data;

    float m_t_fine;
    float m_temperature;
    float m_pressure;
    float m_humidity;
};

#endif /* CBME280_H_ */
