#include "rng_user.h"
#include "portable/portable.h"

void init_rng_user(void)
{
    _init_rng();
}

uint8_t get_rng(uint8_t *rng, uint16_t rng_len)
{
    return _get_rng(rng, rng_len);
}
