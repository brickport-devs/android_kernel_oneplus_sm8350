#include <linux/module.h>

/* init config */
static bool enable = false;
module_param_named(enable, enable, bool, 0644);

static bool debug = false;
module_param_named(debug, debug, bool, 0644);

static unsigned int decay = 80;
module_param_named(decay, decay, uint, 0644);

/* module parameters */
static int cb_config_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops cb_config_ops = {
	.set = cb_config_store,
};
module_param_cb(config, &cb_config_ops, NULL, 0220);

static int cb_dump_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops cb_dump_ops = {
	.get = cb_dump_show,
};
module_param_cb(dump, &cb_dump_ops, NULL, 0444);
