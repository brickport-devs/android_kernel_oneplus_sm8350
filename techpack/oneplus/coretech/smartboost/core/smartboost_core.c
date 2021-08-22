#include <linux/module.h>
#include <linux/mmzone.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/init.h>

static int sysctl_page_cache_reside_switch;
module_param_named(page_cache_reside_switch, sysctl_page_cache_reside_switch, uint, 0644);

static int lowmem_minfree[6] = {
	3 * 512,	/* 6MB */
	2 * 1024,	/* 8MB */
	4 * 1024,	/* 16MB */
	16 * 1024,	/* 64MB */
};

static int lowmem_minfree_size = 4;
module_param_array_named(minfree, lowmem_minfree, uint, &lowmem_minfree_size, S_IRUGO | S_IWUSR);

/*
 * Output information about zones in @pgdat.
 */
static int uid_lru_info_show(struct seq_file *m, void *arg)
{
	return 0;
}

static void *uid_lru_info_start(struct seq_file *m, loff_t *pos)
{
	pg_data_t *pgdat;
	loff_t node = *pos;

	for (pgdat = first_online_pgdat();
	     pgdat && node;
	     pgdat = next_online_pgdat(pgdat))
		--node;

	return pgdat;
}

static void *uid_lru_info_next(struct seq_file *m, void *arg, loff_t *pos)
{
	pg_data_t *pgdat = (pg_data_t *)arg;

	(*pos)++;
	return next_online_pgdat(pgdat);
}

static void uid_lru_info_stop(struct seq_file *m, void *arg)
{
}

static const struct seq_operations uid_lru_info_op = {
	.start	= uid_lru_info_start,
	.next	= uid_lru_info_next,
	.stop	= uid_lru_info_stop,
	.show	= uid_lru_info_show,
};

static int uid_lru_info_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &uid_lru_info_op);
}

static const struct file_operations proc_uid_lru_info_file_operations = {
	.open		= uid_lru_info_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init smartboost_init(void)
{
	proc_create("uid_lru_info", 0444, NULL,
				&proc_uid_lru_info_file_operations);

	return 0;
}

module_init(smartboost_init);
