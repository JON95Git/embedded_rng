#ifndef __APP_H
#define __APP_H

#include <stdint.h>
#include "portable/portable.h"

typedef enum{
    OP_RETURN_RANDOM_IN_A_RANGE = 0,
    OP_CREATE_FILE_FROM_RANDOM_ATTOMIC,
    OP_CREATE_FILE_FROM_RANDOM_INIT,
    OP_CREATE_FILE_FROM_RANDOM_UPDATE,
    OP_CREATE_FILE_FROM_RANDOM_FINAL,
    //Force size to 32 bits
    _OP_ENUM_END = 0xFFFFFFFF
} app_opcode_t;

/**
 * @brief Returns a random number in a given range
 * @param min minimal value
 * @param max maximum value
 * @param random pointer to a variable that holds the random number
 */
void return_random_in_a_range(uint32_t min, uint32_t max, uint32_t *random);

/**
 * @brief Creates a file in file system from random data
 * @param name file name
 * @param file_size_mb file size in mega bytes
 * @return error code
 */
uint8_t create_file_from_random_data(const char *name, uint32_t file_size_mb);


/**
 * @brief Init the process of creation a file in file system from random data
 * @param file_object pointer to a file object
 * @param name file name
 * @return error code
 */
uint8_t create_file_from_random_data_init(FS_FILE_T **file_object, const char *name);

/**
 * @brief Continues the process of creation a file in file system from random data
 * @param name file name
 * @param rng_buffer_len size of each acquire random data
 * @return error code
 */
uint8_t create_file_from_random_data_update(FS_FILE_T *file_object, uint16_t rng_buffer_len);

/**
 * @brief Finishes the process of creation a file in file system from random data
 * @param file_object pointer to a file object
 * @return error code
 */
uint8_t create_file_from_random_data_final(FS_FILE_T *file_object);

#endif // __APP_H
