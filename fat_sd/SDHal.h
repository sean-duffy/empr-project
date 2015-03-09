/*
 * SDHal.h
 *
 *  Created on: May 20, 2011
 *      Author: Sagar
 */

#ifndef SDHAL_H_
#define SDHAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC17xx.h"

void spiInit();
void spiLoSpeed();
void spiHiSpeed();

uint8_t spiWrite(uint8_t data);
void csLo();
void csHi();


#ifdef __cplusplus
}
#endif

#endif /* SDHAL_H_ */
