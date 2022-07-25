/**
 *@file CBME280.cpp
 *
 */

/*
 * BME280.cpp
 *
 *  Created on: 24 Jul 2022
 *      Author: salavat.magazov
 */

#include <CBME280.h>

#define BME280_ID             0x60
#define BMP280_ID             0x58
#define SOFT_RESET            0xB6
#define SPI_WRITE_MASK        0x80
#define ID_PACKET_SIZE        2
#define CALIB1_PACKET_SIZE    24
#define CALIB2_PACKET_SIZE    1
#define CALIB3_PACKET_SIZE    8
#define SPI_TIMEOUT_MS        100
#define SENSOR_RESET_DELAY_MS 9

#define CONCAT_BYTES(msb, lsb) (((uint16_t)msb << 8) | (uint16_t)lsb)

#define HIGH true
#define LOW  false

/**
 * @brief Construct a sensor driver class instance.
 *
 * @param p_spi Pointer to HAL handler for SPI channel.
 * @param p_slave_select_port Pointer to GPIO port for SS pin.
 * @param slave_select_pin Mask of the SS pin.
 */
CBME280::CBME280(SPI_HandleTypeDef *p_spi,
                 GPIO_TypeDef *p_slave_select_port,
                 uint16_t slave_select_pin)
    : mp_spi(p_spi),
      m_slave_select(p_slave_select_port, slave_select_pin),
      mb_initialised(false)
{
    if (mp_spi == nullptr)
    {
        Error_Handler();
    }
    m_slave_select.set(true);
}

CBME280::~CBME280()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief Initialise the sensor. This must be successfully completed before
 * sensor can be accessed for data.
 *
 * @return True if initialisation is successful, false otherwise.
 */
bool CBME280::init()
{
    /* TODO:
     * 1. Check SPI works.
     * 2. Read chip ID to verify comms to sensor works.
     * 3. Read calibration values and store in appropriate places.
     */
    /* This communication via SPI is going to be done without interrupts.*/
    // TODO: how do we ensure SPI is available? Perhaps need a wrapper around
    // SPI with a mutex.
    if (HAL_SPI_GetState(mp_spi) != HAL_SPI_STATE_READY)
    {
        Error_Handler();
    }
    uint8_t register_address;
    /* Soft reset. */
    register_address = RESET;
    m_slave_select.set(false);
    HAL_SPI_Transmit(mp_spi, &register_address, 1, SPI_TIMEOUT_MS);
    m_slave_select.set(true);
    HAL_Delay(SENSOR_RESET_DELAY_MS);
    /* Read sensor ID to see if it is responding to comms. */
    register_address = ID;
    uint8_t id_register[ID_PACKET_SIZE];
    m_slave_select.set(LOW);
    HAL_SPI_TransmitReceive(mp_spi,
                            &register_address,
                            id_register,
                            ID_PACKET_SIZE,
                            SPI_TIMEOUT_MS);
    m_slave_select.set(HIGH);
    if ((id_register[1] != BME280_ID) && (id_register[1] != BMP280_ID))
    {
        return false;
    }
    /* Read sensor calibration data. */
    register_address = DIG_T1;
    uint8_t calibration_register[CALIB1_PACKET_SIZE +
                                 CALIB2_PACKET_SIZE * CALIB3_PACKET_SIZE];
    m_slave_select.set(LOW);
    HAL_SPI_TransmitReceive(mp_spi,
                            &register_address,
                            calibration_register,
                            CALIB1_PACKET_SIZE,
                            SPI_TIMEOUT_MS);
    m_slave_select.set(HIGH);
    register_address = DIG_H1;
    m_slave_select.set(LOW);
    HAL_SPI_TransmitReceive(mp_spi,
                            &register_address,
                            &(calibration_register[CALIB1_PACKET_SIZE - 1]),
                            CALIB2_PACKET_SIZE,
                            SPI_TIMEOUT_MS);
    m_slave_select.set(HIGH);
    register_address = DIG_H2;
    m_slave_select.set(LOW);
    HAL_SPI_TransmitReceive(mp_spi,
                            &register_address,
                            &(calibration_register[CALIB1_PACKET_SIZE]),
                            CALIB2_PACKET_SIZE,
                            SPI_TIMEOUT_MS);
    m_slave_select.set(HIGH);
    calibrateSensor(calibration_register);
    mb_initialised = true;
    return true;
}

/**
 * @brief Convert raw data from the sensor registers into usable calibration
 * values.
 *
 * @param p_calibration_data Pointer to raw data stream of 33 8-bit values.
 */
void CBME280::calibrateSensor(uint8_t const *const p_calibration_data)
{
    uint8_t const *p_runner = p_calibration_data;
    for (int i = 0; i < T_CALIBRATION_SIZE; i++)
    {
        m_temperature_calibration[i] = *p_runner;
        m_temperature_calibration[i] |= *(++p_runner) << 8;
        ++p_runner;
    }
    for (int i = 0; i < P_CALIBRATION_SIZE; i++)
    {
        m_pressure_calibration[i] = *p_runner;
        m_pressure_calibration[i] |= *(++p_runner) << 8;
        ++p_runner;
    }
    /* Humidity calibration data is spread around and is stored in a weird
     * convoluted way. See the manual. This recovery was copied with
     * modifications from the Bosch github repository.
     * https://github.com/BoschSensortec/BME280_driver */
    /* I am certain the engineer at Bosch who came up with this had management
     * breathing down their neck. One day they stopped caring and made this
     * monstrocity.*/
    m_humidity_calibration[0] = p_calibration_data[25];
    m_humidity_calibration[1] =
        (p_calibration_data[27] << 8) + p_calibration_data[26];
    m_humidity_calibration[2] = p_calibration_data[28];
    m_humidity_calibration[3] =
        (p_calibration_data[29] << 4) + (p_calibration_data[30] & 0x0F);
    m_humidity_calibration[4] =
        (p_calibration_data[31] << 4) + (p_calibration_data[30] >> 4);
    m_humidity_calibration[5] = p_calibration_data[32];
}
