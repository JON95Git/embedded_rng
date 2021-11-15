#ifdef TEST

#include "unity.h"
#include "app.h"
#include "rng_user.h"
#include "portable/portable.h"

#define RNG_FILE_NAME_SUB "rng_file_example_sub"

void setUp(void)
{
    init_rng_user();
}

void tearDown(void)
{
}

void test_get_a_random_number_in_a_range(void)
{
    uint32_t min = 0;
    uint32_t max = 32768;
    uint32_t random;

    return_random_in_a_range(min, max, &random);
    TEST_ASSERT_GREATER_OR_EQUAL(min, random);
    TEST_ASSERT_LESS_OR_EQUAL(max, random);
}

void test_create_file_from_random_data_in_atomic_mode(void)
{
    uint8_t ret = 0;

    ret = create_file_from_random_data("rng_file_example", 1);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_create_file_from_random_data_in_suboperations(void)
{
    uint8_t ret = 0;
    uint8_t percent_index;
    uint8_t file_size_mb = 1;
    uint16_t rng_buffer_len = 1024;
    FS_FILE_T *file_object = NULL;

    ret = create_file_from_random_data_init(&file_object, RNG_FILE_NAME_SUB);
    TEST_ASSERT_EQUAL(0, ret);

    for (uint32_t i = 0; i < file_size_mb * rng_buffer_len; i++) {
        ret = create_file_from_random_data_update(file_object, rng_buffer_len);
        TEST_ASSERT_EQUAL(0, ret);
    }

    ret = create_file_from_random_data_final(file_object);
    TEST_ASSERT_EQUAL(0, ret);
}

#endif // TEST
