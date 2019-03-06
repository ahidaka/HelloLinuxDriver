#
TARGET = period.ko workq.ko workq2.ko kthread.ko tasklet.ko

all: ${TARGET}

period.ko: period.c
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 modules

workq.ko: workq.c
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 modules

workq2.ko: workq2.c
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 modules

kthread.ko: kthread.c
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 modules

tasklet.ko: tasklet.c
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 modules

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1 clean

obj-m := period.o workq.o workq2.o kthread.o tasklet.o

clean-files := *~ *.o *.ko *.mod.[co] Module.*
