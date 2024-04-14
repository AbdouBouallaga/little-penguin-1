#include <linux/version.h>
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

#include <linux/proc_fs.h>   // file operations
#include <linux/seq_file.h>  // seq_read, ...
#include <linux/slab.h>

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
  .proc_lseek = seq_lseek,
  .proc_release = single_release,
};
#else
static const struct file_operations mymounts_ops = {
  .owner = THIS_MODULE,
  .open = mymounts_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};
#endif

static int mymounts_show(struct seq_file *m, void *v) {
  struct file *file;
  char buf[4096];
  ssize_t n;

 

  // Open /proc/mounts file
  file = filp_open("/proc/mounts", O_RDONLY, 0);
  if (IS_ERR(file)) {
    return PTR_ERR(file);
  }

  // Read from /proc/mounts
  n = kernel_read(file, buf, sizeof(buf) - 1, &file->f_pos);

  // Close file
  filp_close(file, NULL);

  if (n < 0) {
    return n;
  }

  // Null-terminate the buffer
  buf[n] = '\0';

  // Write content to seq_file buffer
  seq_printf(m, "%s", buf);

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

