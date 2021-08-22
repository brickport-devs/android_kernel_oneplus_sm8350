#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cred.h>
#include <linux/list.h>
#include <linux/slab.h>

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static int null_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops null_store_ops = {
	.set = null_store,
};
module_param_cb(tpd_cmds, &null_store_ops, NULL, 0664);
module_param_cb(tpd_id, &null_store_ops, NULL, 0664);
module_param_cb(tpd_dynamic, &null_store_ops, NULL, 0664);

static struct kernel_param_ops null_show_ops = {
	.get = null_show,
};
module_param_cb(tgid_list, &null_show_ops, NULL, 0664);
module_param_cb(miss_list, &null_show_ops, NULL, 0664);

static atomic_t tpd_enable_rc = ATOMIC_INIT(0);
static int tpd_enable_rc_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&tpd_enable_rc));
}

static struct kernel_param_ops tpd_enable_rc_ops = {
	.get = tpd_enable_rc_show,
};

module_param_cb(tpd_en_rc, &tpd_enable_rc_ops, NULL, 0664);

static int tpd_log_lv = 2;
module_param_named(log_lv, tpd_log_lv, int, 0664);

static atomic_t tpd_ctl = ATOMIC_INIT(0); /* used to ignore fg task checking */

static int tpd_enable = 0;

static int tpd_enable_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", tpd_enable);
}

static struct kernel_param_ops tpd_enable_ops = {
	.set = null_store,
	.get = tpd_enable_show,
};

module_param_cb(tpd_enable, &tpd_enable_ops, NULL, 0664);

static int tpd_ctl_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&tpd_ctl));
}

static struct kernel_param_ops tpd_ctl_ops = {
	.set = null_store,
	.get = tpd_ctl_show,
};

module_param_cb(tpd_ctl, &tpd_ctl_ops, NULL, 0664);



