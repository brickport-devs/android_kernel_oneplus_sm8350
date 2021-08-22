#include <linux/module.h>

#define PCC_PARAMS 4

static int pcclog_lv = 1;
module_param_named(pcclog_lv, pcclog_lv, int, 0664);

// param@1: enable or not, param@2: select_fd_mode, param@3: depress mode, param@4: depress level
static unsigned int params[PCC_PARAMS] = { 0, 0, 0, 0 };
module_param_array_named(params, params, uint, NULL, 0664);

static unsigned int op_cross_limit = 99;
module_param_named(op_cross_limit, op_cross_limit, uint, 0664);

