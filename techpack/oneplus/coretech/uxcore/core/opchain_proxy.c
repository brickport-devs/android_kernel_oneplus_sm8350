#include <linux/module.h>

static unsigned int __read_mostly boost;
module_param(boost, uint, 0644);
static unsigned int __read_mostly boost_tl;
module_param(boost_tl, uint, 0644);
static unsigned int __read_mostly boost_sample_time = 1;
module_param(boost_sample_time, uint, 0644);
static unsigned int __read_mostly chain_on = 0;
module_param(chain_on, uint, 0644);
static unsigned int __read_mostly latest_ms = 100;

static int latest_ms_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE,	"%u", latest_ms);
}

static int latest_ms_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static const struct kernel_param_ops param_ops_latest_ms = {
	.get = latest_ms_show,
	.set = latest_ms_store,
};

module_param_cb(latest_ms, &param_ops_latest_ms, NULL, 0644);
