#ifndef __RNG_USER_H
#define __RNG_USER_H

#include <stdint.h>

void init_rng_user(void);
uint8_t get_rng(uint8_t *buff, uint16_t buff_len);

#endif /* __RNG_USER_H */