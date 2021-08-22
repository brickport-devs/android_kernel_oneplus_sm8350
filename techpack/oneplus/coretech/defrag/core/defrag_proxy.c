#include <linux/module.h>

static unsigned int __read_mostly disable;

static int disable_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%u", disable);
}

static int disable_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static const struct kernel_param_ops param_ops_disable = {
	.get = disable_show,
	.set = disable_store,
};
module_param_cb(disable, &param_ops_disable, NULL, 0644);

static int fp_fail_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static const struct kernel_param_ops param_ops_fp_fail = {
	.get = fp_fail_show,
};
module_param_cb(fp_fail, &param_ops_fp_fail, NULL, 0444);
