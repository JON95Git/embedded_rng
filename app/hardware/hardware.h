#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "main.h"

void hardware_init(void);
uint32_t timer_get_current_tick(void);
void timer_update_callback(void);

#endif /* __HARDWARE_H */