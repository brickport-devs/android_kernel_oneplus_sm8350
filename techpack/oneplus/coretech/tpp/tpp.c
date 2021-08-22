#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

/*
 * log output
 * lv == 0 -> verbose info
 * lv == 1 -> some infomation
 * lv == 2 -> warning
 * lv == 3 -> error
 */
static int tpp_log_lv = 1;
module_param_named(log_lv, tpp_log_lv, int, 0664);

// Set tpp_on 1 to open tpp features
static int tpp_on = 0;
module_param_named(tpp_on, tpp_on, int, 0664);

static int tpp_task_record_on = 0;

static int tpp_strategy;
module_param_named(strategy, tpp_strategy, int, 0664);

static int null_store(const char *buf, const struct kernel_param *kp)
{
	return 0;
}

static int null_show(char *buf, const struct kernel_param *kp)
{
	return 0;
}

static inline int tpp_task_record_on_show(char *buf, const struct kernel_param *kp)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", tpp_task_record_on);
}

static struct kernel_param_ops tpp_task_record_on_ops = {
	.set = null_store,
	.get = tpp_task_record_on_show,
};

module_param_cb(tpp_task_record_on, &tpp_task_record_on_ops, NULL, 0664);

static struct kernel_param_ops null_show_ops = {
	.get = null_show,
};
module_param_cb(wk_thrd_cnt, &null_show_ops, NULL, 0664);
module_param_cb(strategy_names, &null_show_ops, NULL, 0664);

static struct kernel_param_ops null_store_ops = {
	.set = null_store,
};
module_param_cb(clear_threads_cnt, &null_store_ops, NULL, 0664);

static struct kernel_param_ops null_ops = {
	.set = null_store,
	.get = null_show,
};
module_param_cb(tpp_cpu_select_monitor_index, &null_ops, NULL, 0664);
module_param_cb(tpp_task_monitor_index, &null_ops, NULL, 0664);

static int null_show_report(struct seq_file *m, void *v)
{
	return 0;
}

static int null_open_report(struct inode *inode, struct file *file)
{
	return single_open(file, null_show_report, NULL);
}

static const struct file_operations null_report_proc_fops = {
	.open = null_open_report,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init tpp_init(void)
{
	proc_create("tpp_task_report", S_IFREG | 0444, NULL,
		    &null_report_proc_fops);
	proc_create("tpp_tagged_list", S_IFREG | 0444, NULL,
		    &null_report_proc_fops);
	proc_create("tpp_cpu_select_report", S_IFREG | 0444, NULL,
		    &null_report_proc_fops);
	return 0;
}

pure_initcall(tpp_init);
