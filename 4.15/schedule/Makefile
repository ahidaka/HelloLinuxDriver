#
EXTRA_CFLAGS := -DDEBUG=1

#if yocto for raspberrypi3
#KDIR = /home/hidaka/rp3/build/rpi3/tmp/work/raspberrypi3-poky-linux-gnueabi/linux-raspberrypi/1_4.14.79+gitAUTOINC+9ca74c53cb-r0/linux-raspberrypi3-standard-build/

KDIR = /lib/modules/`uname -r`/build

MODS = period.ko workq.ko workq2.ko kthread.ko tasklet.ko
OBJS = period.o workq.o workq2.o kthread.o tasklet.o


TARGETS = ${MODS}

all: ${TARGETS}

period.ko: period.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

workq.ko: workq.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

workq2.ko: workq2.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

kthread.ko: kthread.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

tasklet.ko: tasklet.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

clean:
	make -C $(KDIR) M=`pwd` clean

obj-m := ${OBJS}

clean-files := *~ *.o *.ko *.mod.[co] Module.*
