#include <linux/module.h>

static int alloc_wait_h_ms = 50;
static int alloc_wait_l_ms = 10;
static int alloc_wait_log_ms = 1000;
static int alloc_wait_trig_ms = 10000;

module_param_named(alloc_wait_h_ms, alloc_wait_h_ms, int, S_IRUGO | S_IWUSR);
module_param_named(alloc_wait_l_ms, alloc_wait_l_ms, int, S_IRUGO | S_IWUSR);
module_param_named(alloc_wait_log_ms, alloc_wait_log_ms, int, S_IRUGO | S_IWUSR);
module_param_named(alloc_wait_trig_ms, alloc_wait_trig_ms, int, S_IRUGO | S_IWUSR);
