int sdcardfs_mount(char *hw, char *type, long flags, char *data,int f);
//DIRENT *sdcardfs_readdir(DIR *dirp,int mode);
int sdcardfs_isatty(int fd);
int sdcardfs_create(char *path,int mode);
int sdcardfs_stat(int fd,void *stat);
int sdcardfs_access(char *path,int mode);
int sdcardfs_lseek(int fd,int offset,int whence);
int sdcardfs_init(int dev);
void sdcardfs_sync(int blk);
void sdcard_system_init(void);
int sdcardfs_ioctl(int dev,int req,void *pt);
int sdcardfs_close(int dev);
int sdcardfs_open(int dev,int flags);
int sdcardfs_write(int dev,char *buf,int length);
int sdcardfs_read(int dev,char *buf,int length);

int sdcardfs_mkdir(char *path,int mode);
//int sdcardfs_unlink(STAT *stat);

typedef struct __SDCARD_POINTERS {
	char 	mmc_card_inserted;		// Was the mount sucsesful, is fo mark it foe later
	FATFS 	fatfs[1];		// File system object for each logical drive 
	char 	Lfname[_MAX_LFN + 1];		// Long file name
	int	p1,s1,s2;			// need to be persistent over calls, hence staying here
}SDCARD_DATA;

extern char mmc_card_inserted;

#ifdef CONFIG_SDCARD_TEST_CMD
int sdcard_test_command(int c, char **a);
#endif
