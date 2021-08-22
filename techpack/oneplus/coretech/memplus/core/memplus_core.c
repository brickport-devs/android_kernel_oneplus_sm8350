#include <linux/module.h>

/* -1 = system free to use swap, 0 = disable retention, swap not available, 1 = enable retention */
static int vm_memory_plus __read_mostly = 0;
module_param_named(memory_plus_enabled, vm_memory_plus, uint, S_IRUGO | S_IWUSR);
static unsigned long memplus_add_to_swap = 0;
module_param_named(memplus_add_to_swap, memplus_add_to_swap, ulong, S_IRUGO | S_IWUSR);

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops null_store_ops = {
	.set = null_store,
};

module_param_cb(memory_plus_test_worstcase, &null_store_ops, NULL, 0200);
module_param_cb(memory_plus_wake_gcd, &null_store_ops, NULL, 0644);

static unsigned int memex_threshold __read_mostly;

static unsigned int vm_cam_aware __read_mostly = 1;
module_param_named(memory_plus_cam_aware, vm_cam_aware, uint, S_IRUGO | S_IWUSR);

static unsigned int vm_swapin __read_mostly = 0;
module_param_named(memory_plus_swapin, vm_swapin, uint, S_IRUGO | S_IWUSR);

static int memory_plus_wake_memex_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", memex_threshold);
}

static struct kernel_param_ops memory_plus_wake_memex_ops = {
	.set = null_store,
	.get = memory_plus_wake_memex_show,
};
module_param_cb(memory_plus_wake_memex, &memory_plus_wake_memex_ops, NULL, 0644);
