#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>

static int sw;
static struct workqueue_struct *wq2;
static struct work_struct work;
static struct wq {
	int num;
	char *name;
} wq2_data;

static void deferred_print(void *data)
{
	struct wq *wp;

	wp = (struct wq *) data;
	printk("wq2(%d): deferred_print(%s) = %d [%s]\n",
	       get_cpu(), wp->name, wp->num, current->comm);
	put_cpu();
}

/*
 * called when user is writing
 */
int wq2_set(const char *val, struct kernel_param *kp)
{
	int l = simple_strtol(val, NULL, 0);
	struct wq *datap = &wq2_data;

	*((int *)kp->arg) = l;
	printk("wq2: write(%d)[%s]: sw = %d\n",
	       get_cpu(), current->comm, l);
	put_cpu();
	datap->name = "SET";
	datap->num = l;
        queue_work(wq2, &work);
        queue_work(wq2, &work);
	return 0;
}

/*
 * called when user is reading
 */
int wq2_get(char *buffer, struct kernel_param *kp)
{
	int l = *((int *)kp->arg);
	struct wq *datap = &wq2_data;

	printk("wq2: read(%d)[%s]: sw = %d\n",
	       get_cpu(), current->comm, l);
	put_cpu();
	datap->name = "PUT";
	datap->num = l;
        queue_work(wq2, &work);
        queue_work(wq2, &work);
	return sprintf(buffer, "%d", l);
}

static int __init wq2_init_module(void)
{
    printk(KERN_INFO "workq2: init data = %d\n", sw);
    wq2 = create_workqueue("wq2d");
    if (!wq2)
	    panic("Failed to create wq2d\n");
    INIT_WORK(&work, deferred_print, &wq2_data);
    return 0;
}

static void __exit wq2_cleanup_module(void)
{
    destroy_workqueue(wq2);
    printk(KERN_INFO "workq2: cleanup data = %d\n", sw);
}

MODULE_DESCRIPTION("Sample Wq2 Module");
MODULE_LICENSE("GPL");
module_param_call(sw, wq2_set, wq2_get, &sw, 0644);
MODULE_INFO(parmtype, "sw:int");
MODULE_PARM_DESC(sw, "workq2 special sw");

module_init(wq2_init_module);
module_exit(wq2_cleanup_module);
