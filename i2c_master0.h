/*
 * i2c_master0.h
 *
 *  Created on: 11 ene 2024
 *      Author: Hezitzaile
 */

#ifndef i2c_MASTER_H_
#include "hal_data.h"
#define i2c_MASTER_H_

fsp_err_t init_i2c(void);
void initialice_LCD(void);
void clear_i2c(void);
fsp_err_t write_LCD(uint8_t line[], uint8_t msg[]);


#endif /* I2C_MASTER0_H_ */
