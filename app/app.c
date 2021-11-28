#include <stdio.h>
#include "app.h"
#include "rng_user/rng_user.h"

#define RNG_BUFFER_SIZE 1024

void return_random_in_a_range(uint32_t min, uint32_t max, uint32_t *random)
{
    uint8_t temp_random;

    get_rng(&temp_random, sizeof(temp_random));
    *random = (temp_random % (max - min + 1)) + min;
}

uint8_t create_file_from_random_data(const char *name, uint32_t file_size_mb)
{
    fs_err_t ret_fs = FS_SUCCESS;
    uint8_t ret = 0;
    uint8_t rng[RNG_BUFFER_SIZE];
    uint32_t bytes_written;
    FS_FILE_T *file_object = NULL;

    ret_fs = _sd_fs_create_file(name, &file_object);
    if (ret_fs != FS_SUCCESS) {
        return 1;
    }

    for (uint32_t i = 0; i < file_size_mb * RNG_BUFFER_SIZE; i++) {
        ret = get_rng(rng, sizeof(rng));
        if (ret != 0) {
            printf("get_rng ERROR \n\r");
            return 1;
        }
        
        ret_fs = _sd_fs_write_file(file_object, rng, sizeof(rng) - 1, &bytes_written);
        if (ret_fs != FS_SUCCESS) {
            printf("_sd_fs_close_file ERROR \n\r");
            return 1;
        }
        
        ret_fs = _sd_fs_flush(file_object);
        if (ret_fs != FS_SUCCESS) {
            printf("_sd_fs_close_file ERROR \n\r");
            return 1;
        }
    }

    ret_fs = _sd_fs_close_file(file_object);
    if (ret_fs != FS_SUCCESS) {
        printf("_sd_fs_close_file ERROR \n\r");
        return 1;
    }
    return ret;
}

uint8_t create_file_from_random_data_init(FS_FILE_T **file_object, const char *name)
{
    fs_err_t ret_fs = FS_SUCCESS;
    uint8_t ret = 0;

    ret_fs = _sd_fs_create_file(name, file_object);
    if (ret_fs != FS_SUCCESS) {
        return 1;
    }
    return ret;
}

uint8_t create_file_from_random_data_update(FS_FILE_T *file_object, uint16_t rng_buffer_len)
{
    fs_err_t ret_fs = FS_SUCCESS;
    uint8_t ret = 0;
    uint8_t rng[rng_buffer_len];
    uint32_t bytes_written;

    ret = get_rng(rng, sizeof(rng));
    if (ret != 0) {
        return 1;
    }
 
    ret_fs = _sd_fs_write_file(file_object, rng, sizeof(rng) - 1, &bytes_written);
    if (ret_fs != FS_SUCCESS) {
        return 1;
    }
    return ret;
}

uint8_t create_file_from_random_data_final(FS_FILE_T *file_object)
{
    fs_err_t ret_fs = FS_SUCCESS;
    uint8_t ret = 0;

    ret_fs = _sd_fs_close_file(file_object);
    if (ret_fs != FS_SUCCESS) {
        return ret;
    }
    return ret;
}
