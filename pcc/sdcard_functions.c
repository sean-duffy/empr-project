#include "stdio.h"
#include "types.h"
#include "stdlib.h"
#include "malloc.h"
#include "shell.h"
#include "errno.h"			// System error numbers
#include "ff.h"
#include "spi.h"

#include "sdcard_functions.h"
#include "fatfs/diskio.h"

void map_disk_initialize(int ch)
{
	disk_initialize(ch);
}
void map_spiInit(void)
{
	spiInit();
}

#ifdef NOT_WANTED

typedef struct __SDCARD_POINTERS {
	char 	mmc_card_inserted;		// Was the mount sucsesful, is fo mark it foe later
	FATFS 	fatfs[_VOLUMES];		// File system object for each logical drive 
	char 	Lfname[_MAX_LFN + 1];		// Long file name
}SDCARD_DATA;

extern char mmc_card_inserted;

extern SDCARD_DATA *sdcard_data;

SDCARD_DATA *sdcard_data;

/****************************************************************/
/*					  			*/
/* Mount the SD card on the machine, to a known mount point	*/
/*					  			*/
/****************************************************************/
void mountsdcard(void)
{
	int c;
	FATFS *fs;
	long p1;
	
	disk_initialize(0);
	
	spiInit();
	sdcard_data = (SDCARD_DATA *)mymalloc(sizeof(SDCARD_DATA));	// Create the data structure
	f_mount(0, &sdcard_data->fatfs[0]);
	c = f_getfree("", (DWORD*)&p1, &fs);
	sdcard_data->mmc_card_inserted = (c == FR_OK);
	if (!sdcard_data->mmc_card_inserted)
		printf("Card Mount Error Code=%d\n",c);
	else 
		printf("File system mounted\n");
}
/****************************************************************/
/*					  			*/
/* Give the user a directory listing of the SD card		*/
/*					  			*/
/****************************************************************/
void printdir(void)
{
	DIR dir;				/* Directory object */
	FILINFO finfo;
//	FATFS *fs;
	long p1,p2;
	unsigned res;
	int s1,s2;
//	char *ptr;

	res = f_opendir(&dir, "");
	if (res) { 
		printf("dir response code: %d\n",res); 
		return; 
	}
	p1 = 0;
	p1 = s1 = s2 = 0;
	for(;;) {
		res = f_readdir(&dir, &finfo);
		if ((res != FR_OK) || !finfo.fname[0]) 
			break;
		if (finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++; 
			p1 += finfo.fsize;
		}
		printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s",
			(finfo.fattrib & AM_DIR) ? 'D' : '-',
			(finfo.fattrib & AM_RDO) ? 'R' : '-',
			(finfo.fattrib & AM_HID) ? 'H' : '-',
			(finfo.fattrib & AM_SYS) ? 'S' : '-',
			(finfo.fattrib & AM_ARC) ? 'A' : '-',
			(finfo.fdate >> 9) + 1980, (finfo.fdate >> 5) & 15, finfo.fdate & 31,
			(finfo.ftime >> 11), (finfo.ftime >> 5) & 63,
			finfo.fsize, &(finfo.fname[0]));
		for (p2 = strlen(finfo.fname); p2 < 14; p2++)
			printf(" ");
		printf("%s\n", sdcard_data->Lfname);
			
	}
	printf("%4u File(s),%10lu bytes total\n%4u Dir(s)\n", s1, p1, s2);
	
#ifdef NOT_WANTED
	res = f_opendir(&dir, "");
	if (res) { 
		printf("dir response code: %d\n",res); 
		return; 
	}
	 for (;;) {
        	res = f_readdir(&dir, &finfo);        	/* Read a directory item */
        	if (res || !finfo.fname[0]) 
        		break;    			/* Error or end of dir */
        	if (finfo.fattrib & AM_DIR)
            		printf(" <dir>  %s\n", finfo.fname);
        	else
            		printf("%8lu  %s\n", finfo.fsize, finfo.fname);
    	}
#endif	
//	if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
//		printf(", %10lu bytes free\n", p1 * fs->csize * 512);
}

#endif // Outer NOT_WANTED
