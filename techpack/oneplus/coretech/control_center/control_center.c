#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>

#define CC_CTL_VERSION "2.0"
#define CC_CTL_NODE "cc_ctl"
#define CC_IOC_MAGIC 'c'
#define CC_IOC_MAX 1

static bool cc_time_measure = true;
module_param_named(time_measure, cc_time_measure, bool, 0644);

/* boost enable options */
static bool cc_cpu_boost_enable = true;
module_param_named(cpu_boost_enable, cc_cpu_boost_enable, bool, 0644);

static bool cc_ddr_boost_enable = true;
module_param_named(ddr_boost_enable, cc_ddr_boost_enable, bool, 0644);

static bool cc_fps_boost_enable = true;
module_param_named(fps_boost_enable, cc_fps_boost_enable, bool, 0644);

/* turbo boost */
static bool cc_tb_freq_boost_enable = true;
module_param_named(tb_freq_boost_enable, cc_tb_freq_boost_enable, bool, 0644);

static bool cc_tb_place_boost_enable = true;
module_param_named(tb_place_boost_enable, cc_tb_place_boost_enable, bool, 0644);

static bool cc_tb_nice_last_enable = false;
module_param_named(tb_nice_last_enable, cc_tb_nice_last_enable, bool, 0644);

static int cc_tb_nice_last_period = 10; /* 10 jiffies equals to 40 ms */
module_param_named(tb_nice_last_period, cc_tb_nice_last_period, int, 0644);

static int cc_tb_idle_block_period = 10; /* 10 jiffies equals to 40 ms */
module_param_named(tb_idle_block_period, cc_tb_idle_block_period, int, 0644);

static unsigned long cc_expect_ddrfreq;
module_param_named(expect_ddrfreq, cc_expect_ddrfreq, ulong, 0644);

/*
 * verbose output
 * lv: 0 -> verbose
 * lv: 1 -> info
 * lv: 2 -> wraning
 * lv: 3 -> error
 */
static int cc_log_lv = 1;
module_param_named(log_lv, cc_log_lv, int, 0644);

/* systrace trace marker */
static int cc_systrace_enable = 0;
module_param_named(systrace_enable, cc_systrace_enable, int, 0644);

static int cc_ctl_show(struct seq_file *m, void *v)
{
	seq_printf(m, "control center version: %s\n", CC_CTL_VERSION);
	return 0;
}

static int cc_ctl_open(struct inode *ip, struct file *fp)
{
	return single_open(fp, cc_ctl_show, NULL);;
}

static int cc_ctl_close(struct inode *ip, struct file *fp)
{
	return 0;
}

static long cc_ctl_ioctl(struct file *file, unsigned int cmd, unsigned long __user arg)
{
	if (_IOC_TYPE(cmd) != CC_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > CC_IOC_MAX) return -ENOTTY;

	return 0;
}

static const struct file_operations cc_ctl_fops = {
	.owner = THIS_MODULE,
	.open = cc_ctl_open,
	.release = cc_ctl_close,
	.unlocked_ioctl = cc_ctl_ioctl,
	.compat_ioctl = cc_ctl_ioctl,
	.read = seq_read,
	.llseek = seq_lseek,
};

static const struct file_operations cc_ctl_proc_fops = {
	.owner = THIS_MODULE,
	.open = cc_ctl_open,
	.release = cc_ctl_close,
	.unlocked_ioctl = cc_ctl_ioctl,
	.compat_ioctl = cc_ctl_ioctl,
	.read = seq_read,
	.llseek = seq_lseek,
};

/* TODO try to simplify the register flow */
static dev_t cc_ctl_dev;
static struct class *driver_class;
static struct cdev cdev;
static int cc_cdev_init(void)
{
	int rc;
	struct device *class_dev;

	rc = alloc_chrdev_region(&cc_ctl_dev, 0, 1, CC_CTL_NODE);
	if (rc < 0)
		return 0;

	driver_class = class_create(THIS_MODULE, CC_CTL_NODE);
	if (IS_ERR(driver_class)) {
		rc = -ENOMEM;
		goto exit_unreg_chrdev_region;
	}

	class_dev = device_create(driver_class, NULL, cc_ctl_dev, NULL, CC_CTL_NODE);
	if (IS_ERR(class_dev)) {
		rc = -ENOMEM;
		goto exit_destroy_class;
	}

	cdev_init(&cdev, &cc_ctl_fops);
	cdev.owner = THIS_MODULE;
	rc = cdev_add(&cdev, MKDEV(MAJOR(cc_ctl_dev), 0), 1);
	if (rc < 0) {
		goto exit_destroy_device;
	}

	return 0;

exit_destroy_device:
	device_destroy(driver_class, cc_ctl_dev);
exit_destroy_class:
	class_destroy(driver_class);
exit_unreg_chrdev_region:
	unregister_chrdev_region(cc_ctl_dev, 1);
	return 0;
}

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static int null_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static struct kernel_param_ops null_ops = {
	.set = null_store,
	.get = null_show,
};
module_param_cb(tb_idle_block_enable, &null_ops, NULL, 0644);
module_param_cb(tb_cctl_boost_enable, &null_ops, NULL, 0644);

static struct kernel_param_ops null_show_ops = {
	.get = null_show,
};
module_param_cb(dump_list, &null_show_ops, NULL, 0644);
module_param_cb(freq_show, &null_show_ops, NULL, 0644);
module_param_cb(dump_status, &null_show_ops, NULL, 0644);
module_param_cb(ccdm_status, &null_show_ops, NULL, 0644);
module_param_cb(dump_record, &null_show_ops, NULL, 0644);
module_param_cb(dump_stat, &null_show_ops, NULL, 0644);

static unsigned int ccdm_min_util_threshold = 35;

static int ccdm_min_util_threshold_show(char *buf,
		const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", ccdm_min_util_threshold);
}

static struct kernel_param_ops ccdm_min_thres_ops = {
	.set = null_store,
	.get = ccdm_min_util_threshold_show,
};
module_param_cb(ccdm_min_util_thres, &ccdm_min_thres_ops, NULL, 0664);

static inline void cc_proc_init(void)
{
	proc_create(CC_CTL_NODE, S_IFREG | 0660, NULL, &cc_ctl_proc_fops);
}

static int cc_init(void)
{
	/* FIXME
	 * remove later, so far just for compatible
	 */
	cc_cdev_init(); // create /dev/cc_ctl

	cc_proc_init(); // create /proc/cc_ctl
	return 0;
}

pure_initcall(cc_init);
