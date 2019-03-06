#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>

static int sw;
static struct wq {
	int num;
	char *name;
} workq_data;

static void deferred_print(void *data)
{
	printk("workq(%d): deferred_print = %s [%s]\n",
	       get_cpu(), ((struct wq *)data)->name, current->comm);
	put_cpu();
}

DECLARE_WORK(workq, deferred_print, (void *) &workq_data); 

/*
 * called when user writes the parameter
 */
static int workq_set_sw(const char *val, struct kernel_param *kp)
{
        unsigned long start;
	struct wq *datap = &workq_data;

        if (val && (sw = simple_strtol(val, NULL, 0))) {
		start = jiffies;
		schedule_timeout(sw);
                printk("workq_set(%d)[%s]: sw = %d, time = %lu\n",
                       get_cpu(), current->comm, sw, jiffies - start);
		put_cpu();
		datap->name = "WRITE";
		schedule_work(&workq);
	}
        return(0);
}

/*
 * called when user reads the parameter
 */
static int workq_get_sw(char *buffer, struct kernel_param *kp)
{
	struct wq *datap = &workq_data;

 	printk("workq_get(%d)[%s]: sw = %d\n",
	       get_cpu(), current->comm, sw);
	put_cpu();
	datap->name = "READ";
	schedule_work(&workq);
        return(param_get_int(buffer, kp));
}

static int workq_init_module(void)
{
	printk(KERN_INFO "Workq: init data = %d\n", sw);
	return 0;
}

static void workq_cleanup_module(void)
{
	printk(KERN_INFO "Workq: cleanup data = %d\n", sw);
}

MODULE_DESCRIPTION("Sample Workq Module");
MODULE_LICENSE("GPL");
module_param_call(sw, workq_set_sw, workq_get_sw, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "Sw ON=1/OFF=1: 0-1 (default=0)");

module_init(workq_init_module);
module_exit(workq_cleanup_module);
