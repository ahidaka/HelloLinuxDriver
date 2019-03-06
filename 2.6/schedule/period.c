#include <linux/module.h>
#include <linux/kernel.h>

static int count = 10;
static int sleep = 1000;
static int sw = 0;

/*
 * called when user writes the parameter
 */
static int period_set_sw(const char *val, struct kernel_param *kp)
{
	int i;
	unsigned long start;

	if (val) {
		sw = simple_strtol(val, NULL, 0);
		printk("period_set(%d)[%s]: sw = %d\n",
		       smp_processor_id(), current->comm, sw);
		if (sw > 0) {
			for(i = 0; i < count; i++) {
				start = jiffies;
				set_current_state(TASK_INTERRUPTIBLE);
				schedule_timeout(sleep);
				printk("period_sleep(%i): slept = %lu\n", i, jiffies - start);
			}
		}
	}
	return 0;
}

/*
 * called when user reads the parameter
 */
static int period_get_sw(char *buffer, struct kernel_param *kp)
{
	printk("%s:get: val = <%d>, count =%d\n", kp->name, *((int *)kp->arg), count);
	return(param_get_int(buffer, kp));
}

/*
 * initialize and cleanup
 */
static int period_init(void)
{
	printk(KERN_INFO "period:init count = %d, sleep = %d\n",
	       count, sleep);
	return 0;
}

static void period_exit(void)
{
	printk(KERN_INFO "period:exit count = %d, sleep = %d\n",
	       count, sleep);
}

module_param(count, int, 0644);
MODULE_INFO(parmtype, "count:int");
MODULE_PARM_DESC(count, "Count number: (default=10)"); /* 10times */

module_param(sleep, int, 0644);
MODULE_INFO(parmtype, "sleep:int");
MODULE_PARM_DESC(sleep, "Sleep msec: (default=1000)"); /* 1000ms */

module_param_call(sw, period_set_sw, period_get_sw, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=1: 0-1 (default=0)");

module_init(period_init);
module_exit(period_exit);
MODULE_LICENSE("GPL");
