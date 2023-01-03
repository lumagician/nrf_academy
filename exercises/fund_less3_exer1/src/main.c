#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "myfunction.h"

void main(void) 
{
	int a = 3;
	int b = 4;

	while(1) 
	{
		printk("The Sum of %d and %d is %d\n\r", a, b, sum(a,b));
		k_msleep(1000);
	}
}
