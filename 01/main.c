#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("Hello world module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

// Custom init and exit methods
static int __init custom_init(void) {
    printk(KERN_INFO "Hello world !\n");
    return 0;
}

static void __exit custom_exit(void) {
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(custom_init);
module_exit(custom_exit);
