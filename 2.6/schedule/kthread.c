#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

static int count = 10;
static int sleep = 1000;
static int sw = 0;

struct task_struct *k;

/*
 *
 */
static void kthtest_thread(void)
{
        unsigned long start;
	printk("kthtest_thread(%d)[%s/%s]: sw = %d\n",
	       smp_processor_id(), current->comm, k->comm, sw);
	start = jiffies;
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(sleep);
	printk("kthtest_thread: slept = %lu\n", jiffies - start);
}

/*
 *
 */
static int kthtest_func(void *arg)
{
	int i;

	printk("kthtest_func enter count=%d\n", count);

	for(i = 0; i < count; i++) {
		if (sw == 0) {
			printk("kthtest_func stop by sw == 0\n");
			break;
		}
		else if (kthread_should_stop()) {
			printk("kthtest_func stop by should_stop\n");
			break;
		}
		printk("kthtest_func(%d:%d)[%s/%s]: sw = %d\n",
		       i, smp_processor_id(), current->comm, k->comm, sw);
		kthtest_thread();
	}
	printk("kthtest_func exit...\n");

	return 0;
}

/*
 * called when user writes the parameter
 */
static int kthtest_set_sw(const char *val, struct kernel_param *kp)
{
        if (val) {
                sw = simple_strtol(val, NULL, 0);
                printk("kthtest_set(%d)[%s]: sw = %d\n",
                       smp_processor_id(), current->comm, sw);

		if (sw > 0) {
			printk("kthtest_set: kthread_run\n");
			k = kthread_run(kthtest_func, NULL, "kthtest kthread");
		}
	}
	return 0;
}

/*
 * called when user reads the parameter
 */
static int kthtest_get_sw(char *buffer, struct kernel_param *kp)
{
	printk("%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
	return(param_get_int(buffer, kp));
}

/*
 * initialize and cleanup
 */
static int kthtest_init(void)
{
	printk(KERN_INFO "kthread:init\n");
	return 0;
}

static void kthtest_exit(void)
{
	printk(KERN_INFO "kthread:exit\n");
}

module_param(count, int, 0644);
MODULE_INFO(parmtype, "count:int");
MODULE_PARM_DESC(count, "Count number: (default=10)"); /* 10times */

module_param(sleep, int, 0644);
MODULE_INFO(parmtype, "sleep:int");
MODULE_PARM_DESC(sleep, "Sleep msec: (default=1000)"); /* 1000ms */

module_param_call(sw, kthtest_set_sw, kthtest_get_sw, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=1: 0-1 (default=0)");

module_init(kthtest_init);
module_exit(kthtest_exit);
MODULE_LICENSE("GPL");
