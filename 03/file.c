// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("Module to fix");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

int do_work(int *my_int, int retval)
{
	int x;
	int y;
	int z;

	y = *my_int;
	for (x = 0; x < *my_int; ++x)
		usleep_range(10, 10);
	if (y < 10) {
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");
		z = x * y;
		return z;
	}
	return 1;
}

static int __init my_init(void)
{
	int x;

	x = 10;
	x = do_work(&x, x);
	return x;
}

static void __exit my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

