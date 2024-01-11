/*
 * hal_entry.h
 *
 *  Created on: 11 ene 2024
 *      Author: Hezitzaile
 */

#ifndef HAL_ENTRY_H_
#define HAL_ENTRY_H_
fsp_err_t icu_init(void);
fsp_err_t icu_enable(void);
void icu_deinit(void);

#include "hal_data.h"


#endif /* HAL_ENTRY_H_ */
