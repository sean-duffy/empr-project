/*
 * diskio.cpp
 *
 *  Created on: May 21, 2011
 *      Author: Sagar
 */

#include "diskio.h"
#include "SD.h"
#include "ff.h"

volatile DSTATUS sdStat = STA_NOINIT;

int assign_drives (int a, int b)
{
	return 0;
}
DSTATUS disk_initialize (BYTE drv)
{
	sdStat = STA_NOINIT;
	if(sdInit() == 0)
		sdStat = 0;

	return sdStat;

}
DSTATUS disk_status (BYTE drv)
{
	return sdStat;
}
DRESULT disk_read (BYTE Drive,BYTE* Buffer,DWORD SectorNumber,BYTE SectorCount)
{
	int i;
	BYTE temp = 0;

	for(i=0; i<SectorCount; i++)
	{
		temp = sdRead((SectorNumber+i),(Buffer+(i*512)));
		if(temp != 0)
			break;
	}
	if(temp != 0)
	{
		sdStat = STA_NOINIT;
		return RES_ERROR;
	}

	return RES_OK;
}

DRESULT disk_write (BYTE Drive,const BYTE* Buffer,DWORD SectorNumber,BYTE SectorCount)
{
	int i;
	BYTE temp = 0;

	for(i=0; i<SectorCount; i++)
	{
		temp = sdWrite((SectorNumber+i),(Buffer+(i*512)));
		if(temp != 0)
			break;
	}
	if(temp != 0)
	{
		sdStat = STA_NOINIT;
		return RES_ERROR;
	}

	return RES_OK;
}

DRESULT disk_ioctl (BYTE Drive,BYTE Command,void* Buffer)
{
	switch(Command)
	{
	case CTRL_SYNC: break;
	case GET_SECTOR_SIZE: *((WORD*)Buffer) = 512; break;
	case GET_SECTOR_COUNT: *((WORD*)Buffer) = 0; break;
	case GET_BLOCK_SIZE: *((WORD*)Buffer) = 512; break;
	case CTRL_ERASE_SECTOR: break;

	}

	if(sdStat == 0)
		return RES_OK;

	return RES_ERROR;
}

// DWORD get_fattime (void)
// {
//	return ((1<<21) | (1<<16));
// }
