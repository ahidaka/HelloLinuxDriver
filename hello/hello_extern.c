#include <linux/module.h>

static int a[4] = {0, 1, 2, 3};
static int i = 0;

int hello_extern(void)
{
	return(a[i++ & 3]);
}
