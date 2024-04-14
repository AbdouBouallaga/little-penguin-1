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
	{ 0xddb8457d, "single_release" },
	{ 0x71f3377f, "seq_lseek" },
	{ 0xdc25093b, "seq_read" },
	{ 0xe15c31f8, "remove_proc_entry" },
	{ 0x92997ed8, "_printk" },
	{ 0x901811fb, "proc_create" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x47ea029, "seq_printf" },
	{ 0xc4eeb8eb, "filp_close" },
	{ 0x7568ee5d, "kernel_read" },
	{ 0xd746c32e, "filp_open" },
	{ 0x24927158, "single_open" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C9F28DE112D321597D61918");
