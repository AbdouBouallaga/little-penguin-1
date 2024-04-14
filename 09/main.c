/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymounts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:57:03 by vismaily          #+#    #+#             */
/*   Updated: 2023/11/04 19:30:11 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("List mount points on the system to  /proc/mymounts file.");
MODULE_VERSION("0.0.0.0.0.0.1");
MODULE_LICENSE("GPL");

static struct proc_dir_entry	*mymounts;

static int procfs_show(struct seq_file *m, void *v)
{
	struct mount	*mnt;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		if (strcmp(mnt->mnt_devname, "rootfs") == 0)
	    	return ; // Skip the virtual file system (VFS) mount (itself).

	    struct path mnt_path = {
	    	dentry	: mnt->mnt.mnt_root,
	    	mnt	: &mnt->mnt
	    };
	    struct super_block *sb = mnt_path.dentry->d_sb;

	    if (sb->s_op->show_devname)
	    	sb->s_op->show_devname(m, mnt_path.dentry);
	    else
	    	seq_puts(m, mnt->mnt_devname ? mnt->mnt_devname : "none");

	    seq_putc(m, ' ');
	    seq_path(m, &mnt_path, " \t\n\\");
	    seq_putc(m, '\n');
	}
	return 0;
}

static int procfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, procfs_show, NULL);
}

static struct proc_ops	proc_fops = {
	proc_open	: procfs_open,
};

static int __init procfs_init(void)
{
	mymounts = proc_create("mymounts", 0444, NULL, &proc_fops);
	if (!mymounts)
	{
		pr_info("Error when creating the proc device.\n");
		return -ENOMEM;
	}
	pr_info("Proc device created.\n");
	return 0;
}

static void __exit procfs_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("Cleaning up the proc module.\n");
}

module_init(procfs_init);
module_exit(procfs_exit);