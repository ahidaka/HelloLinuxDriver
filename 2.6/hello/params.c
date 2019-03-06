#include <linux/module.h>
#include <linux/kernel.h>

static int index = 10;
static int debug, sw;
static char line[PAGE_SIZE];
static char sheet[PAGE_SIZE];
static int table[10] = {0,1,2,3,4,5,6,7,8,9};

/*
 * called when user writes the parameter
 */
static int params_set_debug(const char *val, struct kernel_param *kp)
{
	if (val) 
		printk("%s:set: val = <%ld>, index = %d, sw = %d\n",
		       kp->name, simple_strtol(val, NULL, 0), index, sw);
	return(param_set_int(val, kp));
}

static int params_set_line(const char *val, struct kernel_param *kp)
{
	printk("%s:set: val = <%s>\n", kp->name, val);
	return(param_set_copystring(val, kp));
}

/*
 * called when user reads the parameter
 */
static int params_get_debug(char *buffer, struct kernel_param *kp)
{
	printk("%s:get: val = <%d>, index =%d, sw = %d\n",
	       kp->name, *((int *)kp->arg), index, sw);
	return(param_get_int(buffer, kp));
}

static int params_get_line(char *buffer, struct kernel_param *kp)
{
	struct kparam_string *kps = kp->arg;
	printk("%s:get: val = <%s>\n", kp->name, kps->string);
	return(param_get_string(buffer, kp));
}

/*
 * initialize and cleanup
 */
static int params_init(void)
{
	printk(KERN_INFO "params: index = %d, debug = %d, line = %s\n",
	       index, debug, line);
	return 0;
}

static void params_exit(void)
{
	printk(KERN_INFO "params: index = %d, debug = %d, line = %s\n",
	       index, debug, line);
}

module_param(sw, bool, 0644);
MODULE_INFO(parmtype, "sw:bool");
MODULE_PARM_DESC(sw, "bool parameter: Y/N (default=N(0))");

module_param_call(debug, params_set_debug, params_get_debug, &debug, 0644);
MODULE_INFO(parmtype, "debug:int");
MODULE_PARM_DESC(debug, "Debug level: 0-9 (default=0)");

module_param_string(sheet, sheet, sizeof(sheet), 0644);
MODULE_PARM_DESC(sheet, "Free string: (default = null)");

static struct kparam_string string_line = { sizeof(line), line };
module_param_call(line, params_set_line, params_get_line, &string_line, 0644);
MODULE_INFO(parmtype, "line:string");
MODULE_PARM_DESC(line, "Triggered Free string: (default = null)");

module_param_named(tabsize, index, int, 0644);
MODULE_PARM_DESC(index, "Valid table size: 0-10 (default=10)");

module_param_array(table, int, &index, 0644);
MODULE_PARM_DESC(table, "RW int table array");

module_init(params_init);
module_exit(params_exit);
MODULE_LICENSE("GPL");
