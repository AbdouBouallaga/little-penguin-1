#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x877edfa8, "module_layout" },
	{ 0x6cff938f, "debugfs_remove" },
	{ 0x96b3c464, "debugfs_create_file" },
	{ 0x92997ed8, "_printk" },
	{ 0x1766f5eb, "debugfs_create_dir" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xbb4f4766, "simple_write_to_buffer" },
	{ 0x4dfa8d4b, "mutex_lock" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1B47C058966016B50C686FE");
