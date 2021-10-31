#include "rng_user.h"
#include "portable/portable.h"

void init_rng_user(void)
{

}

uint8_t get_rng(uint8_t *buff, uint16_t buff_len)
{
    uint8_t ret = 0;
    for (uint16_t i = 0; i < buff_len; i++) {
        ret = _get_rng(&buff[i]);
        if (ret != 0) {
            return ret;
        }
    }
    return ret;
}
