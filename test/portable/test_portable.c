#ifdef TEST

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "portable/portable.h"

#define FILE_NAME "file_name_example.txt"
#define FILE_CONTENT "Unity testing file management\n"

void setUp(void)
{
    fflush(stdout);
}

void tearDown(void)
{
    // _sd_fs_delete_file(FILE_NAME);
}

void test_create_file(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;

    ret = _sd_fs_create_file(FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    ret = _sd_fs_close_file(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
}

void test_open_file(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;

    ret = _sd_fs_open_file(FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    ret = _sd_fs_flush(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    
    ret = _sd_fs_close_file(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
}

void test_write_file(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;
    uint32_t bytes_written;
    char buffer[] = FILE_CONTENT;

    ret = _sd_fs_create_file(FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    ret = _sd_fs_write_file(file_object, buffer, sizeof(buffer) - 1, &bytes_written);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);

    ret = _sd_fs_flush(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);

    ret = _sd_fs_close_file(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
}

void test_read_file(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;
    uint8_t buffer_read[32] = {0};
    uint32_t bytes_read;
    char buffer[] = FILE_CONTENT;

    ret = _sd_fs_open_file(FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    ret = _sd_fs_read_file(file_object, buffer_read, sizeof(buffer) - 1, &bytes_read);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(buffer, buffer_read, sizeof(buffer));

    ret = _sd_fs_close_file(file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
}

void test_get_file_size(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;
    uint64_t file_size_in_bytes;

    ret = _sd_fs_open_file(FILE_NAME, &file_object);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(NULL, file_object);

    file_size_in_bytes = _sd_fs_get_file_size(file_object);
    TEST_ASSERT_EQUAL(sizeof(FILE_CONTENT) - 1, file_size_in_bytes);
}

void test_delete_file(void)
{
    fs_err_t ret = FS_SUCCESS;
    FS_FILE_T *file_object = NULL;

    ret = _sd_fs_delete_file(FILE_NAME);
    TEST_ASSERT_EQUAL(FS_SUCCESS, ret);

    ret = _sd_fs_open_file(FILE_NAME, &file_object);
    TEST_ASSERT_NOT_EQUAL(FS_SUCCESS, ret);
    TEST_ASSERT_EQUAL(NULL, file_object);
}

#endif // TEST
