#
EXTRA_CFLAGS := -DDEBUG=1

#
#if yocto for raspberrypi3
#KDIR = /home/hidaka/rp3/build/rpi3/tmp/work/raspberrypi3-poky-linux-gnueabi/linux-raspberrypi/1_4.14.79+gitAUTOINC+9ca74c53cb-r0/linux-raspberrypi3-standard-build/

KDIR = /lib/modules/`uname -r`/build

MODS = hello.ko hello_m.ko params.ko
OBJS = hello.o  hello_m.o  params.o
TARGETS = ${MODS}

all: ${TARGETS}

hello.ko: hello.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

hello_m.ko: hello_multi.c hello_extern.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

params.ko: params.c
	make -C $(KDIR) M=`pwd` KBUILD_VERBOSE=1 modules

clean:
	make -C $(KDIR) M=`pwd` clean

obj-m := ${OBJS}

hello_m-objs := hello_multi.o hello_extern.o

clean-files := *~ *.o *.ko *.mod.[co] ${TARGETS} Module.*
