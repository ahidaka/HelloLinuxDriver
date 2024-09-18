#include <linux/module.h>

static int i = 0;
extern int hello_extern(int v);

int hello_extern(int v)
{
	return v ? (i++ & 0xFF) : (i = 0);
}
