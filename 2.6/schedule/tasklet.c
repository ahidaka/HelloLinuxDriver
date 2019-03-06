#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

static int sleep = 1000;
static int sum = 0;
static int sw = 0;

void taskl_tasklet(unsigned long u);

DECLARE_TASKLET(taskl_tlet, taskl_tasklet, 0);

/*
 * My tasklet
 */
void taskl_tasklet(unsigned long u)
{
	int i;
	unsigned long start;

	start = jiffies;
	/* tasklet cannot sleep! */
	for(i = 0; i < sleep; i++) {
		sum++;
	}
	printk("taskl_tasklet(%d/%d): spent = %lu\n", smp_processor_id(),
	       sum, jiffies - start);
}

/*
 * called when user writes the parameter
 */
static int taskl_set_sw(const char *val, struct kernel_param *kp)
{
	if (val) {
		sw = simple_strtol(val, NULL, 0);
		if (sw > 0) {
			printk("taskl_set(%d)[%s]: sw = %d\n",
			       smp_processor_id(), current->comm, sw);
			sum = 0;
			tasklet_schedule(&taskl_tlet);
		}
		else {
			tasklet_disable(&taskl_tlet);
		}
	}
	return(0);
}

/*
 * called when user reads the parameter
 */
static int taskl_get_sw(char *buffer, struct kernel_param *kp)
{
	printk("%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
	return(param_get_int(buffer, kp));
}

/*
 * initialize and cleanup
 */
static int taskl_init(void)
{
	printk(KERN_INFO "taskl:init sleep = %d\n", sleep);
	return 0;
}

static void taskl_exit(void)
{
	tasklet_kill(&taskl_tlet);
	printk(KERN_INFO "taskl:exit sleep = %d\n", sleep);
}

module_param(sleep, int, 0644);
MODULE_INFO(parmtype, "sleep:int");
MODULE_PARM_DESC(sleep, "Sleep tic: (default=1000)"); /* 1000ms */

module_param_call(sw, taskl_set_sw, taskl_get_sw, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=1: 0-1 (default=0)");

module_init(taskl_init);
module_exit(taskl_exit);
MODULE_LICENSE("GPL");
