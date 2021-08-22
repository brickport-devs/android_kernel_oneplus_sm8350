#include <linux/module.h>

static int tb_rdg_enable = 1;

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static int tb_rdg_enable_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", tb_rdg_enable);
}

static struct kernel_param_ops tb_rdg_enable_ops = {
	.set = null_store,
	.get = tb_rdg_enable_show,
};

module_param_cb(tb_rdg_enable, &tb_rdg_enable_ops, NULL, 0664);

static int tb_rdg_list_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops tb_rdg_list_ops = {
	.set = null_store,
	.get = tb_rdg_list_show,
};
module_param_cb(tb_rdg_list, &tb_rdg_list_ops, NULL, 0664);
