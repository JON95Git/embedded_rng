#include "hardware.h"
#include "stm32746g_discovery.h"

static volatile uint32_t timer_counter = 0;

void hardware_init(void)
{

}

uint32_t timer_get_current_tick(void)
{
	return (timer_counter);
}

void timer_update_callback(void)
{
	++timer_counter;
}
