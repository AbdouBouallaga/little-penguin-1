// SPDX-License-Identifier: BSD-3-Clause

 #include <linux/module.h>
 #include <linux/kernel.h>
 #include <linux/init.h>
 #include <linux/miscdevice.h>
 #include <linux/fs.h>
 #include <linux/slab.h>

MODULE_AUTHOR("babdelka");
MODULE_DESCRIPTION("Module to fix");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static ssize_t	m_rd(struct file *fp, char __user *user,
				size_t size, loff_t *offs);
static ssize_t	myfd_write(struct file *fp, const char __user *user,
				size_t size, loff_t *offs);

static const struct file_operations	myfd_fops = {
	.owner	= THIS_MODULE,
	.read	= &m_rd,
	.write	= &myfd_write
};

static struct miscdevice	myfd_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "reverse",
	.fops	= &myfd_fops
};

static char	str[PAGE_SIZE];

ssize_t	m_rd(struct file *fp, char __user *user, size_t size, loff_t *ofs)
{
	size_t	i;
	size_t	j;
	int	    status;
	char	*tmp;

	if (strlen(str) == 0)
		return 0;
	tmp = kmalloc(sizeof(char) * (strlen(str) + 1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
	for (i = 0, j = strlen(str) - 1; j != 0; --j, ++i)
		tmp[i] = str[j];
	tmp[i++] = str[j--];
	tmp[i] = 0x0;
	status = simple_read_from_buffer(user, size, ofs, tmp, i);
	kfree(tmp);
	return status;
}

ssize_t	myfd_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	ssize_t	res;

	res = simple_write_to_buffer(str, PAGE_SIZE - 1, offs, user, size);
	if (res >= 0)
		str[res++] = 0x0;
	return res;
}

static int __init myfd_init(void)
{
	int retval;

	memset(str, 0, PAGE_SIZE);
	retval = misc_register(&myfd_device);
	return retval;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);
