#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/param.h>
#include <linux/semaphore.h>
//#include <linux/jiffies.h>

static long int loop = 1000000;
static long count = 10;
static long sum = 0;
static long ccount = 0;
static int sw = 0;
static struct semaphore my_semaphore;

void taskl_tasklet(struct tasklet_struct *t);

DECLARE_TASKLET(taskl_tlet, taskl_tasklet);

/*
 * My tasklet
 */
void taskl_tasklet(struct tasklet_struct *t)
{
    unsigned long i;
    unsigned long start;    

    printk("taskl_tasklet(%d)[%ld/%ld]: sw=%d\n",
	   smp_processor_id(), ccount, count, sw);
    
    //start = jiffies; 
    for (i = 0; i < loop; i++) {
        sum++;
    }
    printk("taskl_tasklet(%d/%ld): spent = %lu\n", smp_processor_id(),
           sum, 0 /*jiffies - start */ );

    up(&my_semaphore);
}

/*
 * called when user writes the parameter
 */
static int taskl_set_sw(const char *val, const struct kernel_param *kp)
{
    int i;
	
    if (val) {
        sw = simple_strtol(val, NULL, 0);
        if (sw > 0) {
            printk("taskl_set(%d)[%ld]: sw=%d\n",
                   smp_processor_id(), count, sw);

	    for(i = 0; i < count; i++) {
		sum = 0;
		ccount = i;
		tasklet_schedule(&taskl_tlet);

	        down_interruptible(&my_semaphore);
	    }
	    printk("taskl_set_sw, i=%d\n", i);
	    
        } else {
            tasklet_disable(&taskl_tlet);
        }
    }
    return 0;
}

/*
 * called when user reads the parameter
 */
static int taskl_get_sw(char *buffer, const struct kernel_param *kp)
{
    printk("%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
    return param_get_int(buffer, kp);
}

/*
 * initialize and cleanup
 */
static int __init taskl_init(void)
{
    printk(KERN_INFO "taskl:init loop = %ld\n", loop);
    sema_init(&my_semaphore, 0);
    return 0;
}

static void __exit taskl_exit(void)
{
    tasklet_kill(&taskl_tlet);
    printk(KERN_INFO "taskl:exit loop = %ld\n", loop);
}

module_param(count, long, 0644);
MODULE_INFO(parmtype, "count:long");
MODULE_PARM_DESC(count, "Loop count: (default=10)"); /* 10 times */

module_param(loop, long, 0644);
MODULE_INFO(parmtype, "loop:long");
MODULE_PARM_DESC(loop, "Loop tics: (default=1000000)"); /* about 1,000,000 clock */

static const struct kernel_param_ops taskl_sw_ops = {
    .set = taskl_set_sw,
    .get = taskl_get_sw,
};
module_param_cb(sw, &taskl_sw_ops, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=0: 0-1 (default=0)");

module_init(taskl_init);
module_exit(taskl_exit);
MODULE_LICENSE("GPL");
