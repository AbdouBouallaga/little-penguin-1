#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("Debugfs Module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define MAX_FOO_LEN  PAGE_SIZE
#define LOGIN	"babdelka"
#define LOGIN_LEN	8

static struct dentry *fortytwo_dir;
static struct dentry *id_file;
static struct dentry *jiffies_file;
static struct dentry *foo_file;

static u32 foo_value;
static DEFINE_MUTEX(foo_mutex);

static ssize_t id_read(struct file *file, char __user *buffer, size_t length, loff_t *ppos) {
    int bytes_to_copy;

    if (LOGIN_LEN < length)
        bytes_to_copy = LOGIN_LEN;
    else 
        bytes_to_copy = length;

    if (bytes_to_copy <= 0) {
        return 0; // No data to read
    }
    if (copy_to_user(buffer, LOGIN, bytes_to_copy) != 0) {
        return -EFAULT; // Error copying data to user space
    }
    return bytes_to_copy;
}

static ssize_t id_write(struct file *file, const char __user *buffer, size_t length, loff_t *ppos) {
    
    char local_buffer[LOGIN_LEN];
    if (length != LOGIN_LEN) {
        return -EINVAL; // Invalid argument
    }
    if (copy_from_user(local_buffer, buffer, length))
		return -EIO;
    if (memcmp(local_buffer, LOGIN, LOGIN_LEN) != 0) {
        return -EINVAL; // Invalid argument
    }
    return length;
}

static ssize_t jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char temp[32];
    int len;

    len = snprintf(temp, sizeof(temp), "%lu\n", jiffies);
    return simple_read_from_buffer(buf, count, ppos, temp, len);
}

static ssize_t foo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    ssize_t ret;
    unsigned int len;
    len = strlen(foo_value);
    mutex_lock(&foo_mutex);
    ret = simple_read_from_buffer(buf, count, ppos, &foo_value, PAGE_SIZE);
    mutex_unlock(&foo_mutex);
    return ret;
}

static ssize_t foo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    ssize_t ret;
    unsigned int len;
    len = strlen(buf);
    if (len > MAX_FOO_LEN) {
        return -EINVAL;
    }
    mutex_lock(&foo_mutex);
    ret = simple_write_to_buffer(&foo_value, len, ppos, buf, count);
    mutex_unlock(&foo_mutex);
    return ret;
}

static const struct file_operations id_fops = {
    .read = id_read,
    .write = id_write,
};

static const struct file_operations jiffies_fops = {
    .read = jiffies_read,
};

static const struct file_operations foo_fops = {
    .read = foo_read,
    .write = foo_write,
};

static int __init custom_init(void) {
    fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
    if (!fortytwo_dir) {
        printk(KERN_ALERT "Failed to create debugfs directory\n");
        return -ENODEV;
    }

    id_file = debugfs_create_file("id", 0666, fortytwo_dir, NULL, &id_fops);
    if (!id_file) {
        printk(KERN_ALERT "Failed to create debugfs file: id\n");
        debugfs_remove_recursive(fortytwo_dir);
        return -ENODEV;
    }

    jiffies_file = debugfs_create_file("jiffies", 0444, fortytwo_dir, NULL, &jiffies_fops);
    if (!jiffies_file) {
        printk(KERN_ALERT "Failed to create debugfs file: jiffies\n");
        debugfs_remove_recursive(fortytwo_dir);
        return -ENODEV;
    }

    foo_file = debugfs_create_file("foo", 0644, fortytwo_dir, NULL, &foo_fops);
    if (!foo_file) {
        printk(KERN_ALERT "Failed to create debugfs file: foo\n");
        debugfs_remove_recursive(fortytwo_dir);
        return -ENODEV;
    }

    printk(KERN_INFO "Debugfs subdirectory 'fortytwo' created\n");
    return 0;
}

static void __exit custom_exit(void) {
    debugfs_remove_recursive(fortytwo_dir);
    printk(KERN_INFO "Debugfs subdirectory 'fortytwo' removed\n");
}

module_init(custom_init);
module_exit(custom_exit);
