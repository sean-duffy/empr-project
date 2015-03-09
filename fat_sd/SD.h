/*
 * SD.h
 *
 *  Created on: May 20, 2011
 *      Author: Sagar
 */

#ifndef SD_H_
#define SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "SDHal.h"
#include "LPC17xx.h"

uint8_t sdInit(); // returns 0 on success.
uint8_t sdRead(uint32_t lba,uint8_t* data); // 0-success.
uint8_t sdWrite(uint32_t lba,uint8_t* data); // 0-success.

uint8_t sdCommand(uint8_t index,uint32_t argument,uint8_t* R1resp); // 0 on success. 1 - time out

#ifdef __cplusplus
}
#endif

#endif /* SD_H_ */
