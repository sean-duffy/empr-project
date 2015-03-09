/****************************************************************/
/*					  			*/
/* Device driver for the XXXX device				*/
/*					  			*/
/****************************************************************/
#include "stdio.h"
#include "types.h"
#include "stdlib.h"
#include "strings.h"
#include "errno.h"			// System error numbers
#include "shell.h"
#include "malloc.h"
#include "stdio_device.h"
#include "fs.h"
#include "opendir.h"
// As this is a wrapper for the chanfs code, here is that's codes include
// files, so we should now be able to map ours to its
#include "ff.h"
#include "spi.h"
#include "fatfs/diskio.h"

#include "sdcardfs.h"



static 	SDCARD_DATA 	*sdcard_data;
static	FIL 		File[2];		// File objects  for chanfs
static 	ff_DIR 		Dir;			// Directory object for chanfs
static 	FILINFO 	Finfo;			// File info from chanfs
static 	char 		*ptr;
static int sdcard_file_attrib(int att, char *str);	// Munge the file type data

/****************************************************************/
/*					  			*/
/* Basic function calls for the sdcardfsXXX driver over I2C	*/
/* I2C bus address will be the minor device number		*/
/*					  			*/
/****************************************************************/
int sdcardfs_read(int dev, char *buf, int length)
{
	unsigned int	res, r;
#ifdef CONFIG_SDCARD_DEBUG	
	printf("In SDCARD fs read, Length %d, ",length);
#endif
	if(&File[0]==NULL){				// Check for valif file pointer
		buf[0]=EOF;
		return(-1);
	}
	res = f_read(&File[0], buf, length, &r);	// Read data
	if (buf[r]==0xc1) { 				// Test for chanfs EOF value
		buf[r]=EOF;				// Mark as out EOF
		return(r);
	}
	if (res != FR_OK) { 				// Read returned Error
		printf("Error");			// Tell user
		buf[0]=EOF;				// Flag end of file
		return(res);
	}
	if (r != length) { 				// Check for read under-run
		buf[r+1]=EOF;
		return(r);
	}
	
	return(r);					// Return bytes read
}
int sdcardfs_write(int dev, char *buf, int length)
{
	unsigned char	d;
	
	d = return_minor_number(dev);
	printf("In SDCARD fs write\n");
	return 0;
}
int sdcardfs_open(int dev, int flags)
{
	unsigned char	d;
	
	d = return_minor_number(dev);
	
//#ifdef CONFIG_SDCARD_DEBUG
	printf("In SDCARD fs open\n");
//#endif
	return(0);
}
int sdcardfs_close(int dev)
{
	int	d;
	
	d = f_close(&File[0]);
#ifdef CONFIG_SDCARD_DEBUG
	printf("In SDCARD fs close\n");
#endif
	return(d);
}
int sdcardfs_ioctl(int dev, int req, void *pt)
{
	long 	p1,x,y;
	FATFS 	*fs;
#ifdef CONFIG_SDCARD_DEBUG
	printf("In SDCARD fs ioctl\n");
#endif
	switch(req){					// See what requests the user wants with the file system
		case	FS_GET_SIZE		:	// return to the user the status of the file system
			if(sdcard_data->mmc_card_inserted){
				f_getfree("", (DWORD*)&p1, &fs);
//				printf("Csize = %d, Ssize = %d, Free Clusters = %d\n", fs->csize, 512,fs->free_clust);
//				printf("Number of Clusters %d, Last Cluster %d\n",fs->n_fatent-2,fs->last_clust);
				x = ((fs->csize * 512)*fs->free_clust);
				y = ((fs->csize * 512)*fs->n_fatent-2);
				sys_disk_stat->dsk_free = y - x;
				sys_disk_stat->dsk_used = x;
				sys_disk_stat->dsk_clusters = fs->n_fatent-2;
				sys_disk_stat->dsk_flags = O_RDWR;
				return(0);
			} else {
				sys_disk_stat->dsk_free = 0;
				sys_disk_stat->dsk_used = 0;
				sys_disk_stat->dsk_clusters = 0;
				sys_disk_stat->dsk_flags = O_RDONLY;
				errno = ENOENT;	// No path to device
				return(-1);
			}
		default			:
			errno = ENOIOCTL;
			return(-1);
	}
	return(0);
}
int sdcardfs_init(int dev) 
{   
#ifdef CONFIG_SDCARD_DEBUG
	printf("In SDCARD fs init\n");
#endif
	return(0);
}
/****************************************************************/
/*					  			*/
/* Flush the file system, may be nothing to do for sdcardfs	*/
/*					  			*/
/****************************************************************/
void sdcardfs_sync(int blk)
{
	// should call into The f_sync function flushes the cached information of a writing file.
	// will need some work
}
/****************************************************************/
/*					  			*/
/* Called from syscals/system, to keep all data structures	*/
/* local to this directory					*/
/*					  			*/
/****************************************************************/
void sdcard_system_init(void)
{
	sdcard_data = NULL;
}
/****************************************************************/
/*					  			*/
/* Extended System Calls					*/
/*					  			*/
/****************************************************************/
int sdcardfs_lseek(int fd, int offset, int whence)
{
	printf("In SDCARD fs lseek\n");
	return(0);
}
int sdcardfs_access(char *path, int mode)
{
	int	r;

	if(mode == 0){			// Bit of a work around to get things working
		mode = FA_READ | FA_OPEN_EXISTING;
	}
	r = f_open(&File[0],path,mode);
	return(r);
}
int sdcardfs_stat(int fd, void *stat)
{
	printf("In SDCARD fs stat\n");
	return(0);
}
int sdcardfs_create(char *path,int mode)
{
	printf("In SDCARD fs create\n");
	return(0);
}
int sdcardfs_isatty(int fd)
{
	return(0);
}
DIRENT *sdcardfs_readdir(DIR *dirp,int mode)
{
	unsigned char 	res;
#ifdef CONFIG_SDCARD_DEBUG
	printf("In SDCARD fs readdir\n");
#endif
	if(sdcard_data->mmc_card_inserted){
		if(mode & DIRENT_INIT){
//			printf("DIRENT_INIT\n");				// Called to setup the scratch pointers
			res = f_opendir(&Dir, ptr);
			if (res) { 
				printf("f_opendir returned %d\n",res); 
				return(NULL); 
			}
			sdcard_data->p1 = sdcard_data->s1 = sdcard_data->s2 = 0;
		} else if(mode & DIRENT_READY){
//			printf("DIRENT_READY %d\n",dirp->cnt);
			res = f_readdir(&Dir, &Finfo);
			if ((res != FR_OK) || !Finfo.fname[0]) 
				return(NULL);
			if (Finfo.fattrib & AM_DIR) {
				sdcard_data->s2++;
			} else {
				sdcard_data->s1++;
				sdcard_data->p1 += Finfo.fsize;
			}
			strcpy(dirp->entery->name,Finfo.fname);
			dirp->entery->length = Finfo.fsize;
			dirp->entery->status = sdcard_file_attrib(Finfo.fattrib,dirp->entery->name);
			return(dirp->entery);
		}
		return(NULL);
	} else {
//#ifdef CONFIG_SDCARD_DEBUG
		printf("In SDCARD fs readdir, card not inserted\n");
//#endif
		errno=ENOENT;
		return(0);
	}
}
/****************************************************************/
/*					  			*/
/* Will start the SPI stuff up, when mount is called		*/
/*					  			*/
/****************************************************************/
int sdcardfs_mount(char *hw, char *type, long flags, char *data,int f)
{
	int	c;

#ifdef CONFIG_SDCARD_DEBUG
	printf("sdcardfs_mount Needs Writting\n");
#endif
	disk_initialize(0);
#ifdef CONFIG_SDCARD_MOUNT_DEBUG
	printf("Inited Disk\n");
#endif
	spiInit();
#ifdef CONFIG_SDCARD_MOUNT_DEBUG
	printf("Done SPI init\n");
#endif
	if(sdcard_data==NULL){
		sdcard_data = (SDCARD_DATA *)mymalloc(sizeof(SDCARD_DATA));	// Create the data structure
#ifdef CONFIG_SDCARD_MOUNT_DEBUG
		printf("About to mount\n");
#endif
		c = f_mount(0, &sdcard_data->fatfs[0]);
		sdcard_data->mmc_card_inserted = (c == FR_OK);
		if (!sdcard_data->mmc_card_inserted)
			printf("Card Mount Error Code=%d\n",c);
	}
	return(0);
}
/****************************************************************/
/*					  			*/
/* Extended System Calls					*/
/* create a dir on the current file system  			*/
/*					  			*/
/****************************************************************/
int sdcardfs_mkdir(char *path,int mode)
{
	printf("In SDCARD fs mkdir\n");
	return(0);
}
int sdcardfs_unlink(STAT *stat)
{
	printf("In SDCARD fs ulink\n");
	return(0);
}
/****************************************************************/
/*					  			*/
/* Match the attribs from the scard fs, to the system fs	*/
/*					  			*/
/****************************************************************/
static int sdcard_file_attrib(int att, char *str)
{
	int	l = FILE_READABLE | FILE_WRITEABLE;
	int	x;
	
	switch(att){
		case	AM_RDO		:	/* Read only */
			l = l & ~FILE_WRITEABLE;
			break;
		case	AM_SYS		:	/* System */
			l = l | FILE_TYPE_DEV;
			break;
		case	AM_DIR		:	/* Directory */
			l = l | FILE_DIRECTORY;
			break;
		case	AM_LFN		:	/* LFN entry */
		case	AM_ARC		:	/* Archive */
		case	AM_VOL		:	/* Volume label */
		case	AM_HID		:	/* Hidden */
			break;
	}
	x = file_flags(str);
	l = l | x;				// Mask in new flags
	return(l);
}
#ifdef CONFIG_SDCARD_TEST_CMD
/****************************************************************/
/*					  			*/
/* Test call to prove the code is working, will not normally	*/
/* be compiled in						*/
/*					  			*/
/****************************************************************/
int sdcard_test_command(int c, char **a)
{
	char *ptr;
	unsigned char res;
	long	p1;
	unsigned int	s1,s2;
	FATFS 	*fs;
	while (*ptr == ' ') ptr++;
		res = f_opendir(&Dir, ptr);
//		if (res) { 
//			put_rc(res); 
//			break;
//		}
		p1 = s1 = s2 = 0;
		for(;;) {
			res = f_readdir(&Dir, &Finfo);
			if ((res != FR_OK) || !Finfo.fname[0])
				return(0);
			if (Finfo.fattrib & AM_DIR) {
				s2++;
			} else {
				s1++; p1 += Finfo.fsize;
			}
			printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\n",
					(Finfo.fattrib & AM_DIR) ? 'D' : '-',
					(Finfo.fattrib & AM_RDO) ? 'R' : '-',
					(Finfo.fattrib & AM_HID) ? 'H' : '-',
					(Finfo.fattrib & AM_SYS) ? 'S' : '-',
					(Finfo.fattrib & AM_ARC) ? 'A' : '-',
					(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
					(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
					Finfo.fsize,Finfo.fname);
				}
				printf("%4u File(s),%10lu bytes total\n%4u Dir(s)", s1, p1, s2);
				res = f_getfree(ptr, (DWORD*)&p1, &fs);
	return(0);
}
#endif
