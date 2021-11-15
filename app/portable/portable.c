#include "portable.h"
#include <string.h>

static void _prepare_file_name_to_path(const char *file_name, char *final_name)
{
    char path[MAX_FILE_NAME + 10] = FS_PATH;
    strcat(path, file_name);
    strcpy(final_name, path);
}

#ifndef SIMULATOR
RNG_HandleTypeDef hrng;

void _init_rng(void)
{
}

uint8_t _get_rng(uint8_t *rng, uint16_t rng_len)
{
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t rng_buff;

    //TODO: refactor
    if (rng_len < sizeof(uint32_t)) {
        while (hrng.State != HAL_RNG_STATE_READY);
        ret = HAL_RNG_GenerateRandomNumber(&hrng, &rng_buff);
        memcpy(rng, &rng_buff, rng_len);
        return ret;
    }

    for (uint16_t i = 0; i < rng_len; i += sizeof(uint32_t)) {
        while (hrng.State != HAL_RNG_STATE_READY);
        ret = HAL_RNG_GenerateRandomNumber(&hrng, &rng_buff);
        memcpy(rng + i, &rng_buff, sizeof(uint32_t));
    }
    return ret;
}

fs_err_t _sd_fs_create_file(const char *name, FS_FILE_T **file_oject)
{
    FRESULT fat_res;
    char final_name[MAX_FILE_NAME] = {0};
    FS_FILE_T *aux = NULL;

    aux = MALLOC(sizeof(FS_FILE_T));
    _prepare_file_name_to_path(name, final_name);
    fat_res = FS_FILE_OPEN(aux, (const char *)final_name, FS_TRUNC | FS_RDWR);
    if (fat_res != FR_OK) {
        return FS_ERR_OPEN_ERROR;
    }
    *file_oject = aux;
    return FS_SUCCESS;
}

fs_err_t _sd_fs_delete_file(const char *name)
{
    FRESULT fat_res;
    char final_name[MAX_FILE_NAME] = {0};

    _prepare_file_name_to_path(name, final_name);
    fat_res = FS_FILE_REMOVE((const char *)final_name);
    if (fat_res != FR_OK) {
        return FS_ERR_DELETE_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_open_file(const char *name, FS_FILE_T **file_oject)
{
    FRESULT fat_res;
    char final_name[MAX_FILE_NAME] = {0};
    FS_FILE_T *aux = NULL;

    aux = MALLOC(sizeof(FS_FILE_T));
    _prepare_file_name_to_path(name, final_name);
    fat_res = FS_FILE_OPEN(aux, (const char *)final_name, FS_EXIST | FS_RDWR);
    if ((fat_res == FR_NO_FILE)) {
        return FS_ERR_NO_FILE;
    } else if (fat_res != FR_OK) {
        return FS_ERR_OPEN_ERROR;
    }
    *file_oject = aux;
    return FS_SUCCESS;
}

fs_err_t _sd_fs_read_file(FS_FILE_T *file_oject, uint8_t *buffer,
                          uint16_t buffer_len, uint32_t *bytes_read)
{
    FRESULT fat_res;
    UINT bytes_to_read = (UINT)buffer_len;

    fat_res = FS_FILE_READ(file_oject, (void *)buffer, bytes_to_read, (UINT *)bytes_read);
    if (fat_res != FR_OK) {
        FS_FILE_CLOSE(file_oject);
        return FS_ERR_READ_ERROR;
    }

    if (bytes_to_read > *bytes_read) {
        return FS_ERR_END_OF_FILE_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_write_file(FS_FILE_T *file_oject, uint8_t *buffer,
                           uint16_t buffer_len, uint32_t *bytes_written)
{
    FRESULT fat_res;
    UINT bytes_to_write = (UINT)buffer_len;

    fat_res = FS_FILE_WRITE(file_oject, (void *)buffer, bytes_to_write, (UINT *)bytes_written);
    if (fat_res != FR_OK) {
        FS_FILE_CLOSE(file_oject);
        return FS_ERR_WRITE_ERROR;
    }

    fat_res = FS_FLUSH(file_oject);
    if (fat_res != FR_OK) {
        return FS_ERR_FSYNC_ERROR;
    }

    if (bytes_to_write > *bytes_written) {
        return FS_ERR_VOLUME_FULL_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_close_file(FS_FILE_T *file_oject)
{
    FRESULT fat_res;

    fat_res = FS_FILE_CLOSE(file_oject);
    if (fat_res != FR_OK) {
        return FS_ERR_CLOSE_ERROR;
    }
    FREE(file_oject);
    return FS_SUCCESS;
}

fs_err_t _sd_fs_flush(FS_FILE_T *file_oject)
{
    FRESULT fat_res;

    fat_res = FS_FLUSH(file_oject);
    if (fat_res != FR_OK) {
        return FS_ERR_FSYNC_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_move_file_pointer(FS_FILE_T *file_oject, uint32_t offset)
{
    FRESULT fat_res;
    fat_res = FS_FILE_LSEEK(file_oject, f_tell(file_oject) + offset);
    if (fat_res != FR_OK) {
        FS_FILE_CLOSE(file_oject);
        return FS_ERR_LSEEK_ERROR;
    }
    return FS_SUCCESS;
}

void _sd_fs_reset_file_pointer(FS_FILE_T *file_oject)
{
    FS_REWIND(file_oject);
}

#else

#include <stdio.h>
#include <stdlib.h>

void _init_rng(void)
{
    int i, n;
    time_t t;
    n = 5;
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));
}

uint8_t _get_rng(uint8_t *rng, uint16_t rng_len)
{
    uint8_t ret = 0;
    int rng_buff;

    //TODO: refactor
    if (rng_len < sizeof(int)) {
        rng_buff = rand();
        memcpy(rng, &rng_buff, rng_len);
        return ret;
    }

    for (uint16_t i = 0; i < rng_len; i += sizeof(int)) {
        rng_buff = rand();
        memcpy(rng + i, &rng_buff, sizeof(int));
    }
    return 0;
}

fs_err_t _sd_fs_create_file(const char *name, FS_FILE_T **file_oject)
{
    char final_name[MAX_FILE_NAME] = {0};

    _prepare_file_name_to_path(name, final_name);
    *file_oject = fopen((const char *)final_name, FS_CREAT);
    if (*file_oject == NULL) {
        return FS_ERR_OPEN_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_delete_file(const char *name)
{
    char final_name[MAX_FILE_NAME] = {0};

    _prepare_file_name_to_path(name, final_name);
    return remove(final_name);
}

fs_err_t _sd_fs_open_file(const char *name, FS_FILE_T **file_oject)
{
    char final_name[MAX_FILE_NAME] = {0};

    _prepare_file_name_to_path(name, final_name);
    *file_oject = fopen((const char *)final_name, FS_RDONLY);
    if (*file_oject == NULL) {
        return FS_ERR_OPEN_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_read_file(FS_FILE_T *file_oject, uint8_t *buffer,
                          uint16_t buffer_len, uint32_t *bytes_read)
{
    uint16_t bytes_to_read = (uint16_t)buffer_len;

    *bytes_read = fread((void *)buffer, sizeof(uint8_t), (size_t)bytes_to_read, file_oject);
    if (bytes_to_read > *bytes_read) {
        return FS_ERR_END_OF_FILE_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_write_file(FS_FILE_T *file_oject, uint8_t *buffer,
                           uint16_t buffer_len, uint32_t *bytes_written)
{
    uint16_t bytes_to_write = (uint16_t)buffer_len;

    *bytes_written = fwrite((const void *)buffer, sizeof(uint8_t), (size_t)bytes_to_write, file_oject);
    if (bytes_to_write > *bytes_written) {
        return FS_ERR_VOLUME_FULL_ERROR;
    }
    return FS_SUCCESS;
}

fs_err_t _sd_fs_close_file(FS_FILE_T *file_oject)
{
    int ret;

    ret = fclose(file_oject);
    if (ret != 0) {
        return FS_ERR_CLOSE_ERROR;
    }
    return FS_SUCCESS;
}

uint64_t _sd_fs_get_file_size(FS_FILE_T *file_oject)
{
    uint64_t file_size;
    fseek(file_oject, 0L, SEEK_END);
    file_size = ftell(file_oject);
    rewind(file_oject);
    return file_size;
}

fs_err_t _sd_fs_flush(FS_FILE_T *file_oject)
{
    int ret;
    ret = fflush(file_oject);
    if (ret != 0) {
        return FS_ERR_FSYNC_ERROR;
    }
    return FS_SUCCESS;
}

#endif