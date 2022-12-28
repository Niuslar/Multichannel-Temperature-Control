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
#include <math.h>

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

/* These values were extracted from BME280 calibration source code. The original
 * calibration source code was intended for use with integers to allow its use
 * on resource constrained MCUs. This code will run on MCU with floating point
 * unit, so converting into float calibration increases dynamic range and make
 * code much more readable and maintainable.
 */
constexpr float T_CALIB_CORRECTION[] = {1, 1 / 1024, 1 / sqrt(8092)};
constexpr float P_CALIB_CORRECTION[] = {1 / 6250,
                                        1 / 524288 / 32768,
                                        1 / 524288 / 524288 / 32768,
                                        65536 / 4096,
                                        1 / 2 / 4096,
                                        1 / 32768 / 4 / 4096,
                                        1 / 16,
                                        1 / 32768 / 16,
                                        1 / 2147483648 / 16};
constexpr float H_CALIB_CORRECTION[] =
    {-1 / 524288, 1 / 65536, 1 / 67108864, -1 / 64, 1 / 16384, 1 / 67108864};

constexpr float T_SCALE_FACTOR = 1 / 5120;
constexpr float P_OFFSET_1 = 64000;
constexpr float P_OFFSET_2 = 1048576;
constexpr float H_OFFSET = 76800;

/* Static instance control variables. */
CBME280 *CBME280::sp_sensors[] = {nullptr};
uint8_t CBME280::s_sensor_count = 0;

/**
 * @brief Construct a sensor driver class instance.
 *
 */
CBME280::CBME280() : m_state(INIT_PENDING) {}

CBME280::~CBME280()
{
    // TODO Auto-generated destructor stub
}
/**
 * @brief Begin sensor initialization
 *
 * @param p_spi Pointer to HAL handler for SPI channel.
 * @param p_slave_select_port Pointer to GPIO port for SS pin.
 * @param slave_select_pin Mask of the SS pin.
 * @return
 */
bool CBME280::init(SPI_HandleTypeDef *p_spi,
                   GPIO_TypeDef *p_slave_select_port,
                   uint16_t slave_select_pin)
{
    mp_spi = p_spi;
    m_slave_select.init(p_slave_select_port, slave_select_pin);
    if (mp_spi == nullptr)
    {
        return false;
    }
    m_slave_select.set(true);
    if (s_sensor_count < MAX_SENSORS)
    {
        sp_sensors[s_sensor_count] = this;
        s_sensor_count++;
    }
    else
    {
        return false;
    }
}
/**
 * @brief Finish sensor initialization. This must be successfully completed
 * before sensor can be accessed for data.
 *
 * @return True if initialisation is successful, false otherwise.
 */
bool CBME280::init()
{
    /* TODO:
     * 1. Check SPI available.
     * 2. Read chip ID to verify comms to sensor works.
     * 3. Read calibration values and store in appropriate places.
     */
    /* This communication via SPI is going to be done without interrupts.*/
    // TODO: how do we ensure SPI is available? Perhaps need a wrapper around

    if (HAL_SPI_GetState(mp_spi) != HAL_SPI_STATE_READY)
    {
        return false;
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
    // TODO: sensor requires setting up.
    //  set operational mode.
    //  set oversampling modes for all three measurements.
    return true;
}

bool CBME280::run()
{
    bool b_new_data = false;
    switch (m_state)
    {
        case INIT_PENDING:
            if (init())
            {
                m_state = READY;
            }
            break;
        case READY:
            // TODO: perhaps need to have some sort of timeout here to only run
            // this at regular intervals.
            if (startMeasurement())
            {
                m_state = MEASURING;
            }
            break;
        case MEASURING:
            // N.B. This state is a wait-state to allow class to know when it is
            // busy waiting for SPI interrupt to fire.
            break;
        case NEW_DATA:
            /* The sequence of these method calls must be preserved since
             * temperature is used in pressure and humidity calculations.
             * convertRawData();*/
            calculateT();
            calculateP();
            calculateH();
            b_new_data = true;
            break;
        default:
            Error_Handler();
    }
    return b_new_data;
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
    uint32_t raw_calibration;
    for (int i = 0; i < T_CALIBRATION_SIZE; i++)
    {
        raw_calibration = m_temperature_calibration[i] = *p_runner;
        raw_calibration |= *(++p_runner) << 8;
        m_temperature_calibration[i] = raw_calibration * T_CALIB_CORRECTION[i];
        ++p_runner;
    }
    for (int i = 0; i < P_CALIBRATION_SIZE; i++)
    {
        raw_calibration = *p_runner;
        raw_calibration |= *(++p_runner) << 8;
        m_pressure_calibration[i] = raw_calibration * P_CALIB_CORRECTION[i];
        ++p_runner;
    }
    /* Humidity calibration data is spread around and is stored in a
     * weird convoluted way. See the manual. This recovery was
     * copied with modifications from the Bosch github repository.
     * https://github.com/BoschSensortec/BME280_driver */
    /* I am certain the engineer at Bosch who came up with this had
     * management breathing down their neck. One day they stopped
     * caring and made this monstrocity.*/
    raw_calibration = p_calibration_data[25];
    m_humidity_calibration[0] = raw_calibration * H_CALIB_CORRECTION[0];
    raw_calibration = (p_calibration_data[27] << 8) + p_calibration_data[26];
    m_humidity_calibration[1] = raw_calibration * H_CALIB_CORRECTION[1];
    raw_calibration = p_calibration_data[28];
    m_humidity_calibration[2] = raw_calibration * H_CALIB_CORRECTION[2];
    raw_calibration =
        (p_calibration_data[29] << 4) + (p_calibration_data[30] & 0x0F);
    m_humidity_calibration[3] = raw_calibration * H_CALIB_CORRECTION[3];
    raw_calibration =
        (p_calibration_data[31] << 4) + (p_calibration_data[30] >> 4);
    m_humidity_calibration[4] = raw_calibration * H_CALIB_CORRECTION[4];
    raw_calibration = p_calibration_data[32];
    m_humidity_calibration[5] = raw_calibration * H_CALIB_CORRECTION[5];
}

/**
 * @brief Convert raw data from sensor registers into corresponding
 * unprocessed sensor values.
 *
 * @param p_raw_adc_data Pointer to raw data stream of 8 8-bit
 * values.
 */
void CBME280::convertRawData()
{
    m_raw_pressure_data = m_raw_adc_data[0];
    m_raw_pressure_data = m_raw_pressure_data << 8;
    m_raw_pressure_data += m_raw_adc_data[1];
    m_raw_pressure_data = m_raw_pressure_data << 4;
    m_raw_pressure_data += m_raw_adc_data[2] >> 4;

    m_raw_temperature_data = m_raw_adc_data[0];
    m_raw_temperature_data = m_raw_temperature_data << 8;
    m_raw_temperature_data += m_raw_adc_data[1];
    m_raw_temperature_data = m_raw_temperature_data << 4;
    m_raw_temperature_data += m_raw_adc_data[2] >> 4;

    m_raw_humidity_data = m_raw_adc_data[6];
    m_raw_humidity_data = m_raw_humidity_data << 8;
    m_raw_humidity_data += m_raw_adc_data[7];
}

/**
 * @brief Initiate measurement process.
 *
 * @return True if measurement started successfully.
 */
bool CBME280::startMeasurement()
{
    if (HAL_SPI_GetState(mp_spi) != HAL_SPI_STATE_READY)
    {
        return false;
    }
    /* Configure SPI comms as required by the sensor. */
    // TODO: this type of channel sharing with different
    // configurations should be handled by a wrapper class. It can
    // also queue up traffic and call callback functions.
    mp_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    mp_spi->Init.CLKPhase = SPI_PHASE_2EDGE;
    mp_spi->Init.NSS = SPI_NSS_SOFT;
    mp_spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        return false;
    }
    m_raw_adc_data[0] = PRESS_MSB;
    for (int i = 1; i < RAW_ADC_DATA_SIZE; i++)
    {
        m_raw_adc_data[i] = 0;
    }
    HAL_SPI_TransmitReceive_IT(mp_spi,
                               m_raw_adc_data,
                               m_raw_adc_data,
                               RAW_ADC_DATA_SIZE);
    return true;
}

/**
 * @brief Apply temperature calibration to raw data.
 *
 */
void CBME280::calculateT()
{
    float t;
    t = (m_raw_temperature_data / 16 - m_temperature_calibration[0]);
    m_t_fine =
        t * m_temperature_calibration[1] + t * t * m_temperature_calibration[2];
    m_temperature = m_t_fine / T_SCALE_FACTOR;
}

/**
 * @brief Apply pressure calibration to raw data.
 *
 */
void CBME280::calculateP()
{
    float t_p = m_t_fine / 2 - P_OFFSET_1;
    float var_1 = m_pressure_calibration[3];
    var_1 += t_p * m_pressure_calibration[4];
    var_1 += t_p * t_p * m_pressure_calibration[5];
    float var_2 = 1;
    var_2 += t_p * m_pressure_calibration[1];
    var_2 += t_p * t_p * m_pressure_calibration[2];
    var_2 *= m_pressure_calibration[0];
    float var_3 = P_OFFSET_2 - m_raw_pressure_data - var_1;
    var_3 /= var_2;
    m_pressure = m_pressure_calibration[6];
    m_pressure += var_3 * m_pressure_calibration[7];
    m_pressure += var_3 * var_3 * m_pressure_calibration[8];
}

/**
 * @brief Apply humidity calibration to raw data.
 *
 */
void CBME280::calculateH()
{
    float t_h = m_t_fine - H_OFFSET;
    float var_1 = 1 + t_h * m_humidity_calibration[2];
    var_1 *= t_h * m_humidity_calibration[5];
    var_1 += 1;
    var_1 *= m_humidity_calibration[1];
    var_1 *= m_raw_humidity_data + m_humidity_calibration[3] +
             t_h * m_humidity_calibration[4];
    m_humidity = var_1;
    m_humidity += var_1 * var_1 * m_humidity_calibration[0];
}

/**
 * @brief Process IRQ call for all registered and active sensors.
 *
 */
void CBME280::processIrq()
{
    uint8_t sensor = 0;
    while ((sp_sensors[sensor] != nullptr) && sensor < s_sensor_count)
    {
        if (sp_sensors[sensor]->isMeasuring())
        {
            sp_sensors[sensor]->m_state = NEW_DATA;
            break;
        }
        sensor++;
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    CBME280::processIrq();
}
