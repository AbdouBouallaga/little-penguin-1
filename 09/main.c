#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static int __init list_mounts_init(void) {
    struct mount *mnt;
    struct vfsmount *vfsmnt;

    printk(KERN_INFO "Listing mount points:\n");

    // Traverse the mount namespace
    namespace_lock();
    vfsmnt = current->nsproxy->mnt_ns->root;
    list_for_each_entry(mnt, &vfsmnt->mnt_list, mnt_list) {
        printk(KERN_INFO "%s mounted on %s\n", mnt->mnt_sb->s_type->name, mnt->mnt_mountpoint->d_name.name);
    }
    namespace_unlock();

    return 0;
}

static void __exit list_mounts_exit(void) {
    printk(KERN_INFO "Module unloaded\n");
}

module_init(list_mounts_init);
module_exit(list_mounts_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("List mount points on the system");

