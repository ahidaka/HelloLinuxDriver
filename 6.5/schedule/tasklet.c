#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/param.h>
#include <linux/jiffies.h>

static int sleep = 1000;
static int sum = 0;
static int sw = 0;

void taskl_tasklet(struct tasklet_struct *t);

DECLARE_TASKLET(taskl_tlet, taskl_tasklet);

/*
 * My tasklet
 */
void taskl_tasklet(struct tasklet_struct *t)
{
    int i;
    unsigned long start;

    start = jiffies;
    /* tasklet cannot sleep! */
    for (i = 0; i < sleep; i++) {
        sum++;
    }
    printk("taskl_tasklet(%d/%d): spent = %lu\n", smp_processor_id(),
           sum, jiffies - start);
}

/*
 * called when user writes the parameter
 */
static int taskl_set_sw(const char *val, const struct kernel_param *kp)
{
    if (val) {
        sw = simple_strtol(val, NULL, 0);
        if (sw > 0) {
            printk("taskl_set(%d)[%s]: sw = %d\n",
                   smp_processor_id(), current->comm, sw);
            sum = 0;
            tasklet_schedule(&taskl_tlet);
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
    printk(KERN_INFO "taskl:init sleep = %d\n", sleep);
    return 0;
}

static void __exit taskl_exit(void)
{
    tasklet_kill(&taskl_tlet);
    printk(KERN_INFO "taskl:exit sleep = %d\n", sleep);
}

module_param(sleep, int, 0644);
MODULE_INFO(parmtype, "sleep:int");
MODULE_PARM_DESC(sleep, "Sleep tic: (default=1000)"); /* 1000ms */

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
