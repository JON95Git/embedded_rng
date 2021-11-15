#ifdef TEST

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include "rng_user.h"
#include "portable/portable.h"

#define RNG_BUFFER_SIZE 1024
#define RNG_FILE_SIZE_BYTES 1024000
#define RNG_FILE_NAME "rng.bin"

void setUp(void)
{
    init_rng_user();
}

void tearDown(void)
{
}

void _print_buffer(const char *buff_name, uint8_t *buff, uint16_t buff_len)
{
    printf("%s: ", buff_name);
    for (size_t i = 0; i < buff_len; i++) {
        printf(" 0x%.2x ", buff[i]);
    }
    printf("\n\n");
}

void test_init_rng(void)
{
    init_rng_user();
}

void test_check_rng_sanity(void)
{
    uint8_t rng[32];
    uint8_t rng_aux[32];
    uint8_t buff[32];
    uint8_t ret = 0;

    ret = get_rng(rng, sizeof(rng));
    TEST_ASSERT_EQUAL(0, ret);

    ret = get_rng(rng_aux, sizeof(rng_aux));
    TEST_ASSERT_EQUAL(0, ret);

    TEST_ASSERT_TRUE(memcmp(rng, rng_aux, sizeof(rng)));

    // _print_buffer("rng....", rng, sizeof(rng));
    // _print_buffer("rng_aux", rng_aux, sizeof(rng_aux));
}

void test_create_rng_file(void)
{
    uint8_t ret = 0;
    uint8_t rng[RNG_BUFFER_SIZE];
    uint32_t bytes_written;
    fs_err_t ret_fs = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;

    ret_fs = _sd_fs_create_file(RNG_FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret_fs);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    for (uint16_t i = 0; i < 1000; i++) {
        ret = get_rng(rng, sizeof(rng));
        TEST_ASSERT_EQUAL(0, ret);
        
        ret_fs = _sd_fs_write_file(file_object, rng, sizeof(rng) - 1, &bytes_written);
        TEST_ASSERT_EQUAL(FS_SUCCESS, ret_fs);
    }

    ret_fs = _sd_fs_flush(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret_fs);

    ret_fs = _sd_fs_close_file(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret_fs);
}

#endif // TEST
