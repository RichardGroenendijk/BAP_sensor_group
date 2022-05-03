/*
 * i2c_data.h
 *
 *  Created on: 3 May 2022
 *      Author: richa
 */

#ifndef MAIN_I2C_DATA_H_
#define MAIN_I2C_DATA_H_

#define I2C_MASTER_SCL_IO           18     				       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           19      				   /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0          				   /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SHT35_SENSOR_ADDR                 0x44

#endif /* MAIN_I2C_DATA_H_ */
