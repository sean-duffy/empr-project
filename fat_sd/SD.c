/*
 * SD.cpp
 *
 *  Created on: May 20, 2011
 *      Author: Sagar
 */

#include "SD.h"
#include "SDHal.h"
// pcc #include "delay.h"

uint8_t sdInit()
{
	int i;
	uint8_t resp;
	uint8_t temp;
	spiInit();

	//reset
	csHi();
	for(i=0;i<80;i++)
		spiWrite(0xFF);

	//send CMD0
	csLo();
	temp = sdCommand(0,0,&resp);
	csHi();

	if(temp != 0)
		return 1;
	if(resp != 0x01)
		return 2;

	//send CMD1
	for(i=0;i<16384;i++)
	{
		csLo();
		temp = sdCommand(1,0,&resp);
		csHi();

		if(temp != 0)
			return 3;
		if(resp != 0x01)
		{
			if(resp == 0)
				break;
			else
				return 4;
		}
	}
	if(i==16384)
		return 5;
	spiHiSpeed();
	return 0;
}
uint8_t sdCommand(uint8_t index,uint32_t argument,uint8_t* R1resp)
{
	uint32_t i;
	uint8_t temp;

	// make sure card is not busy.
	for(i=0;i<16384;i++)
		if(spiWrite(0xFF) == 0xFF)
			break;
	if(i==16384)
		return 1;

	// write address/index
	spiWrite(index | 0x40);
	// write argument
	spiWrite((argument >> 24) & 0xFF);
	spiWrite((argument >> 16) & 0xFF);
	spiWrite((argument >> 8) & 0xFF);
	spiWrite(argument & 0xFF);
	// write CRC.
	spiWrite(0x95); // CRC hard-wired to CMD0. In SPI mode, card ignores other CRC for other commands

	// get R1 response
	for(i=0;i<16384;i++)
	{
		temp = spiWrite(0xFF);
		if(temp != 0xFF) // wait for R1 response.
			break;
	}
	spiWrite(0xFF); // dummy byte needed before card can take next command.
	if(i==16384)
	{
		*R1resp = 0;
		return 2;
	}
	*R1resp = temp;
	return 0;
}
uint8_t sdRead(uint32_t lba,uint8_t* data)
{
	uint8_t temp;
	uint8_t resp;
	int i;

	csLo();

	temp = sdCommand(17,lba << 9,&resp);
	if(temp != 0)
	{
		csHi();
		return 1;
	}
	if(resp != 0)
	{
		csHi();
		*data = resp;
		return 2;
	}

	for(i=0;i<16384;i++)
	{
		if(spiWrite(0xff) == 0xFE)
			break;
	}
	if(i==16384)
	{
		csHi();
		return 3;
	}
	for(i=0;i<512;i++)
	{
		data[i] = spiWrite(0xff);
	}
	spiWrite(0xff);
	spiWrite(0xff); // discard CRC
	spiWrite(0xff); // dummy byte b4 sending next command.
	csHi();
	return 0;


}

uint8_t sdWrite(uint32_t lba,uint8_t* data)
{
	uint8_t temp;
	uint8_t resp;
	int i;

	csLo();

	temp = sdCommand(24,lba << 9,&resp);
	if(temp != 0)
	{
		csHi();
		return 1;
	}
	if(resp != 0)
	{
		csHi();
		*data = resp;
		return 2;
	}

	spiWrite(0xff); // Do one dummy transfer

	spiWrite(0xfe); //send data token

	for(i=0;i<512;i++)
	{
		spiWrite(data[i]);
	}
	spiWrite(0xff);
	spiWrite(0xff); // Dummy CRC

	temp = (spiWrite(0xff) & 0x0f);
	spiWrite(0xff);
	csHi();

	if(temp != 5)
		return 3;

	return 0;
}
