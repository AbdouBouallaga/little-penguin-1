#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/namei.h>

typedef int (*iterate_mounts_type)(int(*)(struct vfsmount*, void*), void*, struct vfsmount*);
typedef struct vfsmount *(*collect_mounts_type)(const struct path*);
static void *get_sym(char *name)
{
	unsigned long addr = kallsyms_lookup_name(name);
	printk(KERN_INFO "%s (0x%lx)", name, addr);
	return (void*)addr;
}

static int mount_show(struct vfsmount *mnt, void *v)
{
	struct seq_file *s = v;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };

	if (mnt->mnt_sb->s_op->show_devname)
		mnt->mnt_sb->s_op->show_devname(s, mnt->mnt_root);
	else
		seq_printf(s, "%s", mnt->mnt_sb->s_id);
	seq_putc(s, '\t');
	seq_path(s, &mnt_path, " \t\n\\");
	seq_putc(s, '\n');
	return 0;
}

static int seq_mounts_show(struct seq_file *s, void *v)
{
	struct path path;
	struct vfsmount *mnt;
	int err;
	iterate_mounts_type iterate_mounts = get_sym("iterate_mounts");
	collect_mounts_type collect_mounts = get_sym("collect_mounts");

	err = kern_path("/", 0, &path);
	if (err)
		return (err);
	mnt = collect_mounts(&path);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);
	return iterate_mounts(mount_show, s, mnt);
}

static int mounts_open(struct inode *i, struct file *f)
{
	return single_open(f, &seq_mounts_show, NULL);
}

const static struct proc_ops mounts_operazions = {
	.open = mounts_open,
	.read = seq_read,
};

static struct proc_dir_entry *mount_dir;

static int __init init_lmodule(void)
{
	printk(KERN_INFO "Hello world!\n");
	mount_dir = proc_create("mymounts", 0666, NULL, &mounts_operazions);
	return 0;
}

static void __exit cleanup_lmodule(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	proc_remove(mount_dir);
}

module_init(init_lmodule);
module_exit(cleanup_lmodule);

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("List mount points on the system to  /proc/mymounts file.");
MODULE_VERSION("0.0.0.0.0.0.1");
MODULE_LICENSE("GPL");