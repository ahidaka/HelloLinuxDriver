#include <linux/module.h>
#include <linux/kernel.h>

static int debug;

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world, debug = %d\n", debug);
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye world, debug = %d\n", debug);
}

MODULE_LICENSE("GPL");
module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "Debug level: 0-9 (default=0)");

module_init(hello_init);
module_exit(hello_exit);
