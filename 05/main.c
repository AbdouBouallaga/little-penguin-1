#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME "fortytwo"
#define LOGIN	"babdelka"
#define LOGIN_LEN	8


MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("Minimal Miscellaneous Character Device Driver with Dynamic Minor Number");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static int my_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t my_read(struct file *file, char *buffer, size_t length, loff_t *offset) {
    int bytes_to_copy = min(LOGIN_LEN, length);
    if (bytes_to_copy <= 0) {
        return 0; // No data to read
    }
    if (copy_to_user(buffer, LOGIN, bytes_to_copy) != 0) {
        return -EFAULT; // Error copying data to user space
    }
    return bytes_to_copy;
}

static ssize_t my_write(struct file *file, const char *buffer, size_t length, loff_t *offset) {
    if (length != LOGIN_LEN) {
        return -EINVAL; // Invalid argument
    }
    if (strncmp(buffer, LOGIN, LOGIN_LEN) != 0) {
        return -EINVAL; // Invalid argument
    }
    return length;
    return length;
}

static struct file_operations fops = {
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static struct miscdevice custom_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fops,
};

static int __init custom_init(void) {
    int ret = misc_register(&custom_misc_device);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to register the device\n");
        return ret;
    }
    printk(KERN_INFO "Registered correctly with minor number %d\n", custom_misc_device.minor);
    return 0;
}

static void __exit custom_exit(void) {
    misc_deregister(&custom_misc_device);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(custom_init);
module_exit(custom_exit);
