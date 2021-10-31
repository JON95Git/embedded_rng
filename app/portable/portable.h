#ifndef __PORTABLE_H
#define __PORTABLE_H

#include <stdint.h>

#ifndef SIMULATOR
#include "ff.h"

/* SD card file system wrappers (FatFS)*/
#define FS_APPEND FA_OPEN_APPEND
#define FS_TRUNC FA_CREATE_ALWAYS
#define FS_CREAT FA_CREATE_NEW
#define FS_RDONLY FA_READ
#define FS_WRONLY FA_WRITE
#define FS_RDWR FA_READ | FA_WRITE
#define FS_EXIST FA_OPEN_EXISTING
#define FS_FILE_T FIL
#define FS_FILE_INFO_T FILINFO
#define FS_FILE_OPEN(x, y, z) f_open(x, y, z)
#define FS_FILE_CLOSE(x) f_close( x)
#define FS_FILE_READ(w, x, y, z) f_read(w, x, y, z)
#define FS_FILE_WRITE(w, x, y, z) f_write(w, x, y, z)
#define FS_FILE_REMOVE(x) f_unlink(x)
#define FS_FILE_LSEEK(x, y) f_lseek(x, y)
#define FS_FLUSH(x) f_sync(x)
#define FS_REWIND(x) f_rewind(x)
#define FS_DIR_T DIR
#define FS_DIR_OPEN(x, y) f_opendir(x, y)
#define FS_DIR_CLOSE(x) f_closedir(x)
#define FS_DIR_READ(x,y) f_readdir(x, y)

#else
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

#define FS_APPEND "a+"
#define FS_TRUNC "w+x"
#define FS_CREAT "w+"
#define FS_RDONLY "r"
#define FS_WRONLY "w"
#define FS_RDWR "r+"
#define FS_FILE_T FILE
// #define FS_FILE_INFO_T FILINFO
#define FS_FILE_OPEN(x, y) fopen(x, y)
#define FS_FILE_CLOSE(x) fclose( x)
#define FS_FILE_READ(w, x, y, z) fread(w, x, y)
#define FS_FILE_WRITE(w, x, y, z) fwrite(w, x, y)
#define FS_FILE_REMOVE(x) remove(x)
#define FS_FILE_LSEEK(x, y) fseek(x, y)
#define FS_FLUSH(x) fflush()
#define FS_REWIND(x) rewind(x)
// #define FS_DIR_T DIR
#define FS_DIR_OPEN(x, y) fdopendir(x)
#define FS_DIR_CLOSE(x) fdclosedir(x)
#define FS_DIR_READ(x,y) readdir(x)

#endif

typedef enum {
	FS_SUCCESS = 0,
	FS_ERR_FILESYSTEM_ERROR,
	FS_ERR_OPEN_ERROR,
	FS_ERR_READ_ERROR,
	FS_ERR_WRITE_ERROR,
	FS_ERR_CLOSE_ERROR,
	FS_ERR_DELETE_ERROR,
	FS_ERR_MOUNT_ERROR,
	FS_ERR_UNMOUNT_ERROR,
	FS_ERR_MKFS_ERROR,
	FS_ERR_FDISK_ERROR,
	FS_ERR_VOLUME_FULL_ERROR,
	FS_ERR_END_OF_FILE_ERROR,
	FS_ERR_LSEEK_ERROR,
	FS_ERR_FSYNC_ERROR,
	FS_ERR_TRUNCATE_ERROR,
	FS_ERR_NO_FILE,
	FS_ERR_OPEN_DIR_ERROR,
	FS_ERR_READ_DIR_ERROR,
	//Force size to 32 bits
	_FS_ERR_ENUM_END = 0xFFFFFFFF
} fs_err_t;

uint8_t _get_rng(uint8_t *rng);
fs_err_t _sd_fs_create_file(const char *name, FS_FILE_T **file_oject);
fs_err_t _sd_fs_delete_file(const char *name);
fs_err_t _sd_fs_open_file(const char *name, FS_FILE_T **file_oject);
fs_err_t _sd_fs_read_file(FS_FILE_T *file_oject, uint8_t *buffer,
                            uint16_t buffer_len, uint32_t *bytes_read);
fs_err_t _sd_fs_write_file(FS_FILE_T *file_oject, uint8_t *buffer,
                            uint16_t buffer_len, uint32_t *bytes_written);
fs_err_t _sd_fs_close_file(FS_FILE_T *file_oject);
fs_err_t _sd_fs_flush(FS_FILE_T *file_oject);
fs_err_t _sd_fs_move_file_pointer(FS_FILE_T *file_oject, uint32_t offset);
void _sd_fs_reset_file_pointer(FS_FILE_T *file_oject);

#endif /* __PORTABLE_H */