#ifdef TEST

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include "rng_user.h"
#include "portable/portable.h"

void setUp(void)
{
    int i, n;
    time_t t;
    n = 5;
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));
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

#endif // TEST
