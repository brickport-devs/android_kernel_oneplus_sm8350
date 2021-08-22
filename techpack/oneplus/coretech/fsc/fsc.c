#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static unsigned int fsc_enable = 0;
static bool fsc_details = false;
module_param_named(details, fsc_details, bool, 0644);
static u32 fsc_d_ret = 128;
module_param_named(d_ret, fsc_d_ret, uint, 0644);

/* Set a ceil for fsc obj allocation */
unsigned int fsc_max_val = 100000;
module_param_named(fsc_max_val, fsc_max_val, uint, 0644);

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static int null_show(struct seq_file *m, void *v)
{
	return 0;
}

static struct kernel_param_ops null_store_ops = {
	.set = null_store,
};
module_param_cb(allow_list_add, &null_store_ops, NULL, 0220);
module_param_cb(allow_list_del, &null_store_ops, NULL, 0220);

static int null_open(struct inode *inode, struct file *file)
{
	return single_open(file, null_show, NULL);
}

static const struct file_operations null_fops = {
	.open= null_open,
	.read= seq_read,
	.llseek= seq_lseek,
	.release= single_release,
};

static int fsc_enable_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%u", fsc_enable);
}

static struct kernel_param_ops fsc_enable_ops = {
	.set = null_store,
	.get = fsc_enable_show,
};
module_param_cb(enable, &fsc_enable_ops, NULL, 0644);

static int __init fsc_init(void)
{
	/* info */
	proc_create("fsc_dump", S_IFREG | 0400, NULL, &null_fops);
	proc_create("fsc_allow_list", S_IFREG | 0400, NULL, &null_fops);

	return 0;
}
pure_initcall(fsc_init);
