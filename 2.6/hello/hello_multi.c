#include <linux/module.h>

static int sw = 0;
extern int hello_extern(void);

/*
 * called when user writes the parameter
 */
static int hellom_set_sw(const char *val, struct kernel_param *kp)
{
	int v = hello_extern();

	if (val)
		sw = simple_strtol(val, NULL, 0);
	if (sw) {
		printk("%s:set: val = %d, v = %d\n",
		       kp->name, *((int *)kp->arg), v);
		sw += v;
	}
	return 0;
}

/*
 * called when user reads the parameter
 */
static int hellom_get_sw(char *buffer, struct kernel_param *kp)
{
	printk("%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
	return(param_get_int(buffer, kp));
}

/*
 * initialize and cleanup
 */
static int hellom_init(void)
{
	printk(KERN_INFO "hellom_init\n");
	return 0;
}

static void hellom_exit(void)
{
	printk(KERN_INFO "hellom_exit\n");
}

module_param_call(sw, hellom_set_sw, hellom_get_sw, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=0: 0-1 (default=0)");

MODULE_DESCRIPTION("Sample Hellom Module");
MODULE_SUPPORTED_DEVICE("/dev/null");
MODULE_ALIAS("hello_multi");
MODULE_LICENSE("GPL v2");

module_init(hellom_init);
module_exit(hellom_exit);
