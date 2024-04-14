#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>
#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("idiota");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

static int mymounts_show(struct seq_file *m, void *v);

static int mymounts_open(struct inode *inode, struct file *file);

#ifdef HAVE_PROC_OPS
static const struct proc_ops mymounts_ops = {
  .proc_open = mymounts_open,
  .proc_read = seq_read,
  .proc_release = seq_release,
};
#else
static const struct file_operations mymounts_ops = {
  .owner = THIS_MODULE,
  .open = mymounts_open,
  .read = seq_read,
  .release = seq_release,
};
#endif

static int mymounts_show(struct seq_file *m, void *v) {
    struct file *f;
    char *buf;
    mm_segment_t oldfs;
    int size;

    f = filp_open("/proc/mounts", O_RDONLY, 0);
    if (IS_ERR(f))
        return PTR_ERR(f);

    buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buf) {
        filp_close(f, NULL);
        return -ENOMEM;
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    size = vfs_read(f, buf, PAGE_SIZE, &f->f_pos);
    if (size < 0) {
        kfree(buf);
        filp_close(f, NULL);
        set_fs(oldfs);
        return size;
    }

    seq_printf(m, "%s", buf);

    kfree(buf);
    filp_close(f, NULL);
    set_fs(oldfs);

    return 0;
}

static int mymounts_open(struct inode *inode, struct file *file) {
    return single_open(file, mymounts_show, NULL);
}

static int __init mymounts_init(void) {
    proc_create("mymounts", 0, NULL, &mymounts_ops);
    printk(KERN_INFO "mymounts is mounted\n");
    return 0;
}

static void __exit mymounts_cleanup(void) {
    remove_proc_entry("mymounts", NULL);
    printk(KERN_INFO "mymounts removed\n");
}

module_init(mymounts_init);
module_exit(mymounts_cleanup);
